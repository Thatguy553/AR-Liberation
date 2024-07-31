[EntityEditorProps(category: "GameScripted/ScenarioFramework/Slot", description: "")]
class LIB_ScenarioFrameworkSlotMarkerClass : SCR_ScenarioFrameworkSlotBaseClass
{
}

class LIB_ScenarioFrameworkSlotMarker : SCR_ScenarioFrameworkSlotBase
{
	[Attribute(desc: "Marker Type", category: "Map Marker")]
	protected ref LIB_ScenarioFrameworkMarkerType m_MapMarkerType;
	
	protected ref SCR_MapMarkerBase m_MapMarker;
	
	protected bool m_ScriptSpawned = false;
	protected LIB_EScenarioFrameworkMarkerCustom m_eSpawnedMapMarkerIcon;
	protected LIB_EScenarioFrameworkMarkerCustomColor m_eSpawnedMapMarkerColor;
	protected int m_iSpawnedMapMarkerRotation = 0;
	protected string m_sSpawnedMapMarkerText;
	
	//------------------------------------------------------------------------------------------------
	bool GetScriptSpawned()
	{
		return m_ScriptSpawned;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetScriptSpawned(bool spawned)
	{
		m_ScriptSpawned = spawned;
	}
	
	//------------------------------------------------------------------------------------------------
	LIB_EScenarioFrameworkMarkerCustom GetMapMarkerIcon()
	{
		return m_eSpawnedMapMarkerIcon;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom icon)
	{
		m_eSpawnedMapMarkerIcon = icon;
	}
	
	//------------------------------------------------------------------------------------------------
	LIB_EScenarioFrameworkMarkerCustomColor GetMapMarkerColor()
	{
		return m_eSpawnedMapMarkerColor;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMapMarkerColor(LIB_EScenarioFrameworkMarkerCustomColor color)
	{
		m_eSpawnedMapMarkerColor = color;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetMapMarkerRotation()
	{
		return m_iSpawnedMapMarkerRotation;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMapMarkerRotation(int rotation)
	{
		m_iSpawnedMapMarkerRotation = rotation;
	}
	
	//------------------------------------------------------------------------------------------------
	string GetMapMarkerText()
	{
		return m_sSpawnedMapMarkerText;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetMapMarkerText(string text)
	{
		m_sSpawnedMapMarkerText = text;
	}
	
	
	//------------------------------------------------------------------------------------------------
	//! \return
	LIB_ScenarioFrameworkMarkerType GetMapMarkerType()
	{
		return m_MapMarkerType;
	}
	
	//------------------------------------------------------------------------------------------------
	//! \param[in] type
	void SetMapMarkerType(LIB_ScenarioFrameworkMarkerType type)
	{
		m_MapMarkerType = type;
	}
	
	//------------------------------------------------------------------------------------------------
	//! \return
	SCR_MapMarkerBase GetMapMarker()
	{
		return m_MapMarker;
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	override void RestoreToDefault(bool includeChildren = false, bool reinitAfterRestoration = false)
	{
		RemoveMapMarker();
		m_MapMarker = null;
		
		super.RestoreToDefault(includeChildren, reinitAfterRestoration);
	}
	
	//------------------------------------------------------------------------------------------------
	override void AfterAllChildrenSpawned(SCR_ScenarioFrameworkLayerBase layer)
	{
		if (!m_MapMarker)
			CreateMapMarker();
		
		super.AfterAllChildrenSpawned(this);
	}
	
	//------------------------------------------------------------------------------------------------
	//!
	void RemoveMapMarker()
	{
		if (!m_MapMarker)
			return;
		
		SCR_MapMarkerManagerComponent markerMgr = SCR_MapMarkerManagerComponent.GetInstance();
		if (!markerMgr)
			return;
		
		int markerID = m_MapMarker.GetMarkerID();
		
		SCR_MapMarkerBase marker = markerMgr.GetStaticMarkerByID(markerID);
		if (!marker)
			return;
		
		markerMgr.RemoveStaticMarker(marker);
		m_MapMarker = null;
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateMapMarker()
	{
		SCR_MapMarkerManagerComponent mapMarkerMgr = SCR_MapMarkerManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_MapMarkerManagerComponent));
		if (!mapMarkerMgr)
			return;
		
		m_MapMarker = new SCR_MapMarkerBase();
		
		if (m_ScriptSpawned)
		{
				m_MapMarker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
				m_MapMarker.SetIconEntry(m_eSpawnedMapMarkerIcon);
				m_MapMarker.SetRotation(m_iSpawnedMapMarkerRotation);
				m_MapMarker.SetColorEntry(m_eSpawnedMapMarkerColor);
				m_MapMarker.SetCustomText(m_sSpawnedMapMarkerText);
		} else {
		
			LIB_ScenarioFrameworkMarkerCustom mapMarkerCustom = LIB_ScenarioFrameworkMarkerCustom.Cast(m_MapMarkerType);
			if (mapMarkerCustom)
			{
				m_MapMarker.SetType(SCR_EMapMarkerType.PLACED_CUSTOM);
				m_MapMarker.SetIconEntry(mapMarkerCustom.m_eMapMarkerIcon);
				m_MapMarker.SetRotation(mapMarkerCustom.m_iMapMarkerRotation);
				m_MapMarker.SetColorEntry(mapMarkerCustom.m_eMapMarkerColor);
				m_MapMarker.SetCustomText(m_MapMarkerType.m_sMapMarkerText);
			}
			else
			{
				LIB_ScenarioFrameworkMarkerMilitary mapMarkerMilitary = LIB_ScenarioFrameworkMarkerMilitary.Cast(m_MapMarkerType);
				if (!mapMarkerMilitary)
					return;
				
				m_MapMarker = mapMarkerMgr.PrepareMilitaryMarker(mapMarkerMilitary.m_eMapMarkerFactionIcon, mapMarkerMilitary.m_eMapMarkerDimension, mapMarkerMilitary.m_eMapMarkerType1Modifier | mapMarkerMilitary.m_eMapMarkerType2Modifier);
				m_MapMarker.SetCustomText(m_MapMarkerType.m_sMapMarkerText);
			}
			
		}
		
		vector worldPos = GetOwner().GetOrigin();
		m_MapMarker.SetWorldPos(worldPos[0], worldPos[2]);
		//m_MapMarker.SetCustomText(m_MapMarkerType.m_sMapMarkerText);
		
		FactionManager factionManager = GetGame().GetFactionManager();
		if (factionManager)
		{
			Faction faction = factionManager.GetFactionByKey(m_sFactionKey);
			if (faction)
				m_MapMarker.AddMarkerFactionFlags(factionManager.GetFactionIndex(faction));
		}
		
		mapMarkerMgr.InsertStaticMarker(m_MapMarker, false, true);
	}
}

[BaseContainerProps(), SCR_ContainerActionTitle()]
class LIB_ScenarioFrameworkMarkerType : ScriptAndConfig
{
	[Attribute(desc: "Text which will be displayed for the Map Marker", category: "Map Marker")];
	LocalizedString m_sMapMarkerText;
}

[BaseContainerProps(), SCR_ContainerActionTitle()]
class LIB_ScenarioFrameworkMarkerCustom : LIB_ScenarioFrameworkMarkerType
{
	[Attribute("0", UIWidgets.ComboBox, "Marker Icon", "", ParamEnumArray.FromEnum(LIB_EScenarioFrameworkMarkerCustom), category: "Map Marker")]
	LIB_EScenarioFrameworkMarkerCustom m_eMapMarkerIcon;
	
	[Attribute("0", UIWidgets.ComboBox, "Marker Color", "", ParamEnumArray.FromEnum(LIB_EScenarioFrameworkMarkerCustomColor), category: "Map Marker")]
	LIB_EScenarioFrameworkMarkerCustomColor m_eMapMarkerColor;
	
	[Attribute(defvalue: "0", uiwidget: UIWidgets.Slider, desc: "Rotation of the Map Marker", params: "-180 180 1", category: "Map Marker")]
	int m_iMapMarkerRotation;
}

[BaseContainerProps(), SCR_ContainerActionTitle()]
class LIB_ScenarioFrameworkMarkerMilitary : LIB_ScenarioFrameworkMarkerType
{
	[Attribute(defvalue: EMilitarySymbolIdentity.BLUFOR.ToString(), UIWidgets.ComboBox, "Marker Faction Icon. Not all of these combinations will work as they have to be properly defined in MapMarkerConfig.conf", "", ParamEnumArray.FromEnum(EMilitarySymbolIdentity), category: "Map Marker")]
	EMilitarySymbolIdentity m_eMapMarkerFactionIcon;
	
	[Attribute(defvalue: EMilitarySymbolDimension.LAND.ToString(), UIWidgets.ComboBox, "Marker Dimension. Not all of these combinations will work as they have to be properly defined in MapMarkerConfig.conf", "", ParamEnumArray.FromEnum(EMilitarySymbolDimension), category: "Map Marker")]
	EMilitarySymbolDimension m_eMapMarkerDimension;
	
	[Attribute(defvalue: EMilitarySymbolIcon.INFANTRY.ToString(), UIWidgets.ComboBox, "Marker Type 1 modifier. Not all of these combinations will work as they have to be properly defined in MapMarkerConfig.conf", "", ParamEnumArray.FromEnum(EMilitarySymbolIcon), category: "Map Marker")]
	EMilitarySymbolIcon m_eMapMarkerType1Modifier;
	
	[Attribute(defvalue: EMilitarySymbolIcon.INFANTRY.ToString(), UIWidgets.ComboBox, "Marker Type 2 modifier. Not all of these combinations will work as they have to be properly defined in MapMarkerConfig.conf", "", ParamEnumArray.FromEnum(EMilitarySymbolIcon), category: "Map Marker")]
	EMilitarySymbolIcon m_eMapMarkerType2Modifier;
}

enum LIB_EScenarioFrameworkMarkerCustom
{
	CIRCLE = 0,
	CIRCLE2,
	CROSS,
	CROSS2,
	DOT,
	DOT2,
	DROP_POINT,
	DROP_POINT2,
	ENTRY_POINT,
	ENTRY_POINT2,
	FLAG,
	FLAG2,
	FLAG3,
	FORTIFICATION,
	FORTIFICATION2,
	MARK_EXCLAMATION,
	MARK_EXCLAMATION2,
	MARK_EXCLAMATION3,
	MARK_QUESTION,
	MARK_QUESTION2,
	MARK_QUESTION3,
	MINE_FIELD,
	MINE_FIELD2,
	MINE_FIELD3,
	MINE_SINGLE,
	MINE_SINGLE2,
	MINE_SINGLE3,
	OBJECTIVE_MARKER,
	OBJECTIVE_MARKER2,
	OBSERVATION_POST,
	OBSERVATION_POST2,
	PICK_UP,
	PICK_UP2,
	POINT_OF_INTEREST,
	POINT_OF_INTEREST2,
	POINT_OF_INTEREST3,
	POINT_SPECIAL,
	POINT_SPECIAL2,
	RECON_OUTPOST,
	RECON_OUTPOST2,
	WAYPOINT,
	WAYPOINT2,
	DEFEND,
	DEFEND2,
	DESTROY,
	DESTROY2,
	HEAL,
	HELP,
	HELP2,
	ATTACK,
	ATTACK_MAIN,
	CONTAIN,
	CONTAIN2,
	CONTAIN3,
	RETAIN,
	RETAIN2,
	STRONG_POINT,
	STRONG_POINT2,
	TARGET_REFERENCE_POINT,
	TARGET_REFERENCE_POINT2,
	AMBUSH,
	AMBUSH2,
	RECONNAISSANCE,
	SEARCH_AREA,
	DIRECTION_OF_ATTACK,
	DIRECTION_OF_ATTACK_MAIN,
	DIRECTION_OF_ATTACK_PLANNED,
	FOLLOW_AND_SUPPORT,
	FOLLOW_AND_SUPPORT2,
	JOIN,
	JOIN2,
	JOIN3,
	ARROW_LARGE,
	ARROW_LARGE2,
	ARROW_LARGE3,
	ARROW_MEDIUM,
	ARROW_MEDIUM2,
	ARROW_MEDIUM3,
	ARROW_SMALL,
	ARROW_SMALL2,
	ARROW_SMALL3,
	ARROW_CURVE_LARGE,
	ARROW_CURVE_LARGE2,
	ARROW_CURVE_LARGE3,
	ARROW_CURVE_MEDIUM,
	ARROW_CURVE_MEDIUM2,
	ARROW_CURVE_MEDIUM3,
	ARROW_CURVE_SMALL,
	ARROW_CURVE_SMALL2,
	ARROW_CURVE_SMALL3,
	HQ,
	FUELSTATION,
	TOWN,
	CAPITAL,
	MILITARY,
	TRANSMITTER
}

enum LIB_EScenarioFrameworkMarkerCustomColor
{
	WHITE = 0,
	REFORGER_ORANGE,
	ORANGE,
	RED,
	OPFOR,
	INDEPENDENT,
	GREEN,
	BLUE,
	BLUFOR,
	DARK_BLUE,
	MAGENTA,
	CIVILIAN,
	DARK_PINK
}
