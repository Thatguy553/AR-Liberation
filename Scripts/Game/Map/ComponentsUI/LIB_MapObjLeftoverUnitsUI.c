class LIB_MapObjLeftoverUnitsUI : SCR_MapUIBaseComponent
{
	protected SCR_MapEntity m_MapUnitEntity;
	protected float m_iCycleDuration = 0.1;
	protected float m_fWaitingTime = float.MAX;
	protected bool m_isMapOpen = false;
	protected ref array<IEntity> m_eUnits = {};
	protected ref array<ref Widget> m_UnitMapWidgets = {};

	static ref LIB_MapObjLeftoverUnitsUI m_instance;

	//------------------------------------------------------------------------------------------------
	// constructor
	void LIB_MapObjLeftoverUnitsUI()
	{
		if (!m_instance)
			m_instance = this;

		LIB_ObjectiveComponent.SetUnitMapManager(m_instance);
	}
	
	//------------------------------------------------------------------------------------------------
	//! 
	//! \return
	static LIB_MapObjLeftoverUnitsUI GetInstance()
	{
		return m_instance;
	}

	//------------------------------------------------------------------------------------------------
	override void Init()
	{
		if (!m_instance)
			m_instance = this;
	}

	//------------------------------------------------------------------------------------------------
	//!
	//! \param[in] units
	void AddTrackedUnits(array<IEntity> units)
	{
		m_eUnits.InsertAll(units);
		CreateNewUnitWidgets();
	}

	//------------------------------------------------------------------------------------------------
	//!
	//! \param[in] unit
	void AddTrackedUnits(IEntity unit)
	{
		m_eUnits.Insert(unit);
		CreateNewUnitWidgets();
	}

	//------------------------------------------------------------------------------------------------
	//!
	void CreateNewUnitWidgets()
	{
		if (m_eUnits.Count() <= 0)
			return;

		int widgetCount = m_UnitMapWidgets.Count();
		int unitCount = m_eUnits.Count();
		int widgetsToCreate = unitCount - widgetCount;

		if (widgetsToCreate <= 0)
			return;

		for (int i; i < widgetsToCreate; i++)
		{
			Widget WidgetLayer = GetGame().GetWorkspace().CreateWidgets("{6AE2BB140C18FD7F}UI/layouts/Map/MapUnitMarker.layout", m_RootWidget);
			m_UnitMapWidgets.Insert(WidgetLayer);
		}

	}

	//------------------------------------------------------------------------------------------------
	override void OnMapOpen(MapConfiguration config)
	{
		super.OnMapOpen(config);

		m_MapUnitEntity = SCR_MapEntity.GetMapInstance();
		if (!m_MapUnitEntity)
			return;

		m_isMapOpen = true;
		
		if (m_eUnits.Count() >= 1)
			UpdatePosition();

		m_MapUnitEntity.GetOnMapPan().Insert(LIB_OnMapPan);
		m_MapUnitEntity.GetOnMapZoom().Insert(LIB_OnMapZoom);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnMapClose(MapConfiguration config)
	{
		m_MapUnitEntity.GetOnMapPan().Remove(LIB_OnMapPan);
		m_MapUnitEntity.GetOnMapZoom().Remove(LIB_OnMapZoom);

		foreach (int currentIndex, Widget unitWidget : m_UnitMapWidgets)
		{
			ImageWidget ObjImage = ImageWidget.Cast(unitWidget.FindAnyWidget("Image"));
			
			if(ObjImage)
			{
				ObjImage.SetVisible(false);				
			}
			
			TextWidget ObjText = TextWidget.Cast(unitWidget.FindAnyWidget("Text"));
			if(ObjText)
			{
				ObjText.SetVisible(false);
			}
		}
		
		m_isMapOpen = false;
		super.OnMapClose(config);
	}
	
	//------------------------------------------------------------------------------------------------
	override void Update(float timeSlice)
	{
		if (!m_isMapOpen)
			return;
		m_fWaitingTime += timeSlice;
		if (m_fWaitingTime < m_iCycleDuration)
			return;

		m_fWaitingTime = 0;
		UpdatePosition();
		super.Update(timeSlice);
	}

	//------------------------------------------------------------------------------------------------
	protected void LIB_OnMapPan(float x, float y, bool adjustedPan)
	{
		UpdatePosition();
	}

	//------------------------------------------------------------------------------------------------
	protected void LIB_OnMapZoom(float zoomVal)
	{
		UpdatePosition();
	}

	protected void UpdatePosition()
	{
		if (m_eUnits.Count() <= 0)
			return;
		
		foreach (int currentIndex, IEntity unit : m_eUnits)
		{
			bool UnitAlive = IsAlive(unit);
			vector UnitPos = unit.GetOrigin();
			vector UnitAngles = unit.GetAngles();
			float screenPosX, screenPosY;
			float mapZoom = m_MapUnitEntity.GetCurrentZoom();
			m_MapUnitEntity.WorldToScreen(UnitPos[0], UnitPos[2], screenPosX, screenPosY, true);
			ImageWidget m_UnitImage = ImageWidget.Cast(m_UnitMapWidgets[currentIndex].FindAnyWidget("Image"));
			screenPosX = GetGame().GetWorkspace().DPIUnscale(screenPosX);
			screenPosY = GetGame().GetWorkspace().DPIUnscale(screenPosY);
			if (m_UnitImage)
			{
				m_UnitImage.LoadImageTexture(0, "{067F7C2D05D49085}UI/Textures/Icons/iconman.edds");
				m_UnitImage.SetColor(Color.Red);
				m_UnitImage.SetVisible(UnitAlive);
				FrameSlot.SetPos(
					m_UnitImage,
					screenPosX,
					screenPosY
				);
				m_UnitImage.SetRotation(UnitAngles[1]);
			}

			TextWidget m_UnitText = TextWidget.Cast(m_UnitMapWidgets[currentIndex].FindAnyWidget("Text"));
			if (m_UnitText)
			{
				m_UnitText.SetColor(Color.FromRGBA(0, 0, 0, 255));
				//m_UnitText.SetText(playerManager.GetPlayerName(playerId));
				m_UnitText.SetText("");
				m_UnitText.SetVisible(true);
				FrameSlot.SetPos(
					m_UnitText,
					screenPosX,
					screenPosY
				);
			}
		}

	}
	
	static bool IsAlive(IEntity entity)
	{
	    if (!entity)
	        return false;
	    
	    DamageManagerComponent damageManager;
	    ChimeraCharacter character = ChimeraCharacter.Cast(entity);
	    if (character)
	        damageManager = character.GetDamageManager();
	    else
	        damageManager = DamageManagerComponent.Cast(entity.FindComponent(DamageManagerComponent));
	    
	    if (!damageManager)
	        return true;
	    
	    return damageManager.GetState() != EDamageState.DESTROYED;
	}
}
