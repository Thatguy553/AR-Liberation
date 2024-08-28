// At some point should rename this file and class to something along the lines of LIB_MapObjMarkerUI

class LIB_MapUnitMarkerUI : SCR_MapUIBaseComponent
{
	//protected Widget m_WidgetLayer;
	//protected ImageWidget m_UnitImage;
	protected float m_iCycleDuration = 0.1;
	protected SCR_MapEntity m_MapUnitEntity;
	protected float m_fWaitingTime = float.MAX;
	protected bool m_isMapOpen = false;
	protected ref array<Widget> m_ObjWidgets = {};
	protected LIB_TownManagerComponent m_ObjManager = LIB_TownManagerComponent.GetInstance();

	//------------------------------------------------------------------------------------------------
	override void OnMapOpen(MapConfiguration config)
	{
		super.OnMapOpen(config);
		//Print("--------------------MAPOPEN----------------", LogLevel.NORMAL);
		m_MapUnitEntity = SCR_MapEntity.GetMapInstance();
		if (!m_MapUnitEntity) return;
		
		m_isMapOpen = true;
		m_ObjWidgets = {};
		
		for (int i; i < m_ObjManager.GetTotalObjCount(); i++)
		{
			Widget WidgetLayer = GetGame().GetWorkspace().CreateWidgets("{853AB2F886342179}UI/layouts/Objectives/Obj.layout", m_RootWidget);
			m_ObjWidgets.Insert(WidgetLayer);
		}
		
		m_MapUnitEntity.GetOnMapPan().Insert(TR_OnMapPan);
		m_MapUnitEntity.GetOnMapZoom().Insert(TR_OnMapZoom);
		
		UpdatePosition();
	}
	//------------------------------------------------------------------------------------------------
	override void OnMapClose(MapConfiguration config)
	{
		//Print("--------------------MAPCLOSE----------------", LogLevel.NORMAL);
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
	void TR_OnMapPan(float x, float y, bool adjustedPan)
	{	
		UpdatePosition();
	}
	//------------------------------------------------------------------------------------------------
	void TR_OnMapZoom(float zoomVal)
	{
		UpdatePosition();
	}
	//------------------------------------------------------------------------------------------------
	void UpdatePosition()
	{
		// Print("--------------------MAPUPDATE----------------", LogLevel.NORMAL);
		array<IEntity> allObjs = {};
		allObjs.InsertAll(m_ObjManager.GetHostileObjArray());
		allObjs.InsertAll(m_ObjManager.GetFriendlyObjArray());
		
		LIB_UpdateMapMarkers(allObjs);
	}
	
	void LIB_UpdateMapMarkers(array<IEntity> allObjs)
	{
		foreach (int currentIndex, IEntity obj : allObjs)
		{
			LIB_ObjectiveComponent objComp = LIB_ObjectiveComponent.Cast(obj.FindComponent(LIB_ObjectiveComponent));
			
			FactionKey objFaction = objComp.GetObjFaction();
			
			vector ObjPos = obj.GetOrigin();
			vector ObjAngles = obj.GetAngles();
			float screenPosX, screenPosY;
			float mapZoom = m_MapUnitEntity.GetCurrentZoom();
			
			m_MapUnitEntity.WorldToScreen(ObjPos[0], ObjPos[2], screenPosX, screenPosY, true);
			
			ImageWidget ObjImage = ImageWidget.Cast(m_ObjWidgets[currentIndex].FindAnyWidget("Image0"));
			screenPosX = GetGame().GetWorkspace().DPIUnscale(screenPosX);
			screenPosY = GetGame().GetWorkspace().DPIUnscale(screenPosY);
			
			if(ObjImage)
			{
				switch (objComp.GetObjType())
				{
					case LIB_EObjType.HQ:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "hq");
						break;
					case LIB_EObjType.TOWN:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "town");
						break;
					case LIB_EObjType.CAPITAL:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "capital");
						break;
					case LIB_EObjType.FACTORY:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "fuelstation");
						break;
					case LIB_EObjType.MILITARY:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "military");
						break;
					case LIB_EObjType.RADIO_TOWER:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "military");
						break;
					default:
						ObjImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", "town");
						break;
				}
				
				switch (objFaction)
				{
					case "US":
						ObjImage.SetColor(Color.Blue);
						break;
					case "USSR":
						ObjImage.SetColor(Color.Red);
						break;
				}
				ObjImage.SetVisible(true);
				
				FrameSlot.SetPos(
					ObjImage, 
					screenPosX,
					screenPosY
				);
				ObjImage.SetRotation(ObjAngles[1]);
			}
			
			TextWidget ObjText = TextWidget.Cast(m_ObjWidgets[currentIndex].FindAnyWidget("Text0"));
			if(ObjText)
			{
				ObjText.SetColor(Color.Black);
				ObjText.SetText(objComp.GetObjName());
				ObjText.SetVisible(true);
				float widthT;
				float heightT;
				
				ObjText.GetScreenSize(widthT, heightT);
				
				FrameSlot.SetPos(
					ObjText,
					FrameSlot.GetPos(ObjImage)[0] - (widthT/2), // Attempts to center the text to the marker image
					FrameSlot.GetPos(ObjImage)[1] + 50
				);
			}
			
			// pass the markers for this objective, back to the objective component
			if (objComp.GetMarkerWidgets() == null && ObjImage != null && ObjText != null)
			{
				objComp.SetMarkerWidgets(ObjImage, ObjText);
			}
		}
	}
}