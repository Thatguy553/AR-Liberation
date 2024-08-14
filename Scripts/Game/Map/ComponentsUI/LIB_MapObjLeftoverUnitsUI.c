class LIB_MapObjLeftoverUnitsUI : SCR_MapUIBaseComponent
{
	//protected SCR_MapEntity m_MapUnitEntity;
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
		Print("[LIB] Leftover UI");
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
		Print("Adding Tracked Units");
		Print(units);
		Print(m_eUnits);
		m_eUnits.InsertAll(units);
		CreateNewUnitWidgets();
	}

	//------------------------------------------------------------------------------------------------
	//!
	//! \param[in] unit
	void AddTrackedUnits(IEntity unit)
	{
		Print("Adding Tracked Unit");
		Print(unit);

		m_eUnits.Insert(unit);
		CreateNewUnitWidgets();
	}

	//------------------------------------------------------------------------------------------------
	//!
	void CreateNewUnitWidgets()
	{
		Print("Creating New Widgets");
		if (m_eUnits.Count() <= 0)
			return;

		int widgetCount = m_UnitMapWidgets.Count();
		int unitCount = m_eUnits.Count();
		int widgetsToCreate = unitCount - widgetCount;

		Print(m_eUnits);
		Print(widgetsToCreate);
		if (widgetsToCreate <= 0)
			return;

		for (int i; i < widgetsToCreate; i++)
		{
			Widget WidgetLayer = GetGame().GetWorkspace().CreateWidgets("{6AE2BB140C18FD7E}UI/layouts/Map/MapUnitMarker.layout", m_RootWidget);
			m_UnitMapWidgets.Insert(WidgetLayer);
		}

	}

	//------------------------------------------------------------------------------------------------
	override void OnMapOpen(MapConfiguration config)
	{
		super.OnMapOpen(config);
		m_MapEntity = SCR_MapEntity.GetMapInstance();

		if (!m_MapEntity)
			return;

		m_isMapOpen = true;

		Print("Map Opened");
		Print(m_eUnits);
		Print(m_instance);

		if (m_eUnits.Count() >= 1)
			UpdatePosition();

		m_MapEntity.GetOnMapPan().Insert(LIB_OnMapPan);
		m_MapEntity.GetOnMapZoom().Insert(LIB_OnMapZoom);
	}

	//------------------------------------------------------------------------------------------------
	override protected void OnMapClose(MapConfiguration config)
	{
		m_MapEntity.GetOnMapPan().Remove(LIB_OnMapPan);
		m_MapEntity.GetOnMapZoom().Remove(LIB_OnMapZoom);

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

		Print("Updating Position");
		Print(m_eUnits);

		foreach (int currentIndex, IEntity unit : m_eUnits)
		{

			vector UnitPos = unit.GetOrigin();
			vector UnitAngles = unit.GetAngles();
			float screenPosX, screenPosY;
			float mapZoom = m_MapEntity.GetCurrentZoom();
			m_MapEntity.WorldToScreen(UnitPos[0], UnitPos[2], screenPosX, screenPosY, true);
			ImageWidget m_UnitImage = ImageWidget.Cast(m_UnitMapWidgets[currentIndex].FindAnyWidget("Image"));
			screenPosX = GetGame().GetWorkspace().DPIUnscale(screenPosX);
			screenPosY = GetGame().GetWorkspace().DPIUnscale(screenPosY);
			if (m_UnitImage)
			{
				m_UnitImage.LoadImageTexture(0, "{9731965B995D0B76}UI/Textures/Icons/iconman_ca.edds");
				m_UnitImage.SetColor(Color.Red);
				m_UnitImage.SetVisible(true);
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
}
