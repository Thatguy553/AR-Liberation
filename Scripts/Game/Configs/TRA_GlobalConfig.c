[BaseContainerProps(configRoot: true)]
class TRA_GlobalConfig
{
	// Gamemode related configs
	[Attribute(defvalue: "1", desc: "Should remaining Ai enemy be marked upon base capture?", category: "Gamemode Configs")]
	bool m_RemainingMarker;
	
	// Self explanatory, opfor group prefabs
	[Attribute(defvalue: "", desc: "Opfor Fire Team", category: "Group Prefabs", params: "et")]
	ResourceName m_OFireTeam;
	
	[Attribute(defvalue: "", desc: "Opfor Light Fire Team", category: "Group Prefabs", params: "et")]
	ResourceName m_OLightFireTeam;
	
	[Attribute(defvalue: "", desc: "Opfore MG Team", category: "Group Prefabs", params: "et")]
	ResourceName m_OMgTeam;
	
	[Attribute(defvalue: "", desc: "Opfor Maneuver Group", category: "Group Prefabs", params: "et")]
	ResourceName m_OManeuverGrp;
	
	[Attribute(defvalue: "", desc: "Opfor Medics", category: "Group Prefabs", params: "et")]
	ResourceName m_OMedics;
	
	[Attribute(defvalue: "", desc: "Opfor Rifle Squad", category: "Group Prefabs", params: "et")]
	ResourceName m_ORifleSqd;
	
	[Attribute(defvalue: "", desc: "Opfor AT Team", category: "Group Prefabs", params: "et")]
	ResourceName m_OAtTeam;
	
	[Attribute(defvalue: "", desc: "Opfor Support Team", category: "Group Prefabs", params: "et")]
	ResourceName m_OSuppTeam;

	[Attribute(defvalue: "", desc: "Opfor Infantry Groups Array", category: "Group Prefabs Array")]
	ref array<ResourceName> m_OInfGroups;
	
	[Attribute(defvalue: "", desc: "Opfor BTR", category: "Vehicle Prefabs", params: "et")]
	ResourceName m_OBtr;
	
	[Attribute(defvalue: "", desc: "Opfor UAZ", category: "Vehicle Prefabs", params: "et")]
	ResourceName m_OUaz;
	
	[Attribute(defvalue: "", desc: "Opfor Vehicle Prefabs Array", category: "Vehicle Prefabs Array")]
	ref array<ResourceName> m_OVehGroups;
	
	//------------------------------------------------------------------------------------------------
	// Unit Waypoints
	[Attribute(defvalue: "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointDefend;
	
	[Attribute(defvalue: "{1B0E3436C30FA211}Prefabs/AI/Waypoints/AIWaypoint_Attack.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointAttack;
	
	[Attribute(defvalue: "{712F4795CF8B91C7}Prefabs/AI/Waypoints/AIWaypoint_GetIn.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointGetIn;
	
	[Attribute(defvalue: "{06E1B6EBD480C6E0}Prefabs/AI/Waypoints/AIWaypoint_ForcedMove.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointForcedMove;
	
	[Attribute(defvalue: "{A0509D3C4DD4475E}Prefabs/AI/Waypoints/AIWaypoint_Follow.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointFollow;
	
	[Attribute(defvalue: "{B049D4C74FBC0C4D}Prefabs/AI/Waypoints/AIWaypoint_GetInNearest.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointGetInNearest;
	
	[Attribute(defvalue: "{750A8D1695BD6998}Prefabs/AI/Waypoints/AIWaypoint_Move.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointMove;
	
	[Attribute(defvalue: "{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointPatrol;
	
	[Attribute(defvalue: "{A88F0B6CF25BD1DE}Prefabs/AI/Waypoints/AIWaypoint_Scout.et", desc: "Defend Waypoint", category: "Unit Waypoints")]
	ResourceName m_WaypointScout;
	
	//------------------------------------------------------------------------------------------------
	// Fob related data
	[Attribute(defvalue: "{4BE4C27399CF3B00}Prefabs/Structures/Military/Bunkers/ShelterMilitary_E_01/ShelterMilitary_E_01.et", desc: "Blufor FOB building prefab", category: "Blufor Fob Data")]
	ResourceName m_BFobPrefab;
	
	[Attribute(defvalue: "5", desc: "Distance from the player the preview/obj will be.", category: "Fob Data")]
	float m_BPreviewDist;
	
	[Attribute(desc: "Build list item data", category: "Fob Data")]
	ref array<ref LIB_BuildItemData> m_BFobBuildList;
	
	[Attribute(desc: "Ordered List of FOB names", category: "Fob Data")]
	ref array<string> m_BFobNameList;
	
	//------------------------------------------------------------------------------------------------
	static ref TRA_GlobalConfig m_Config;
	
	static TRA_GlobalConfig GetConfig(ResourceName ConfigRes = "{80A853CA085D4CD2}Configs/Global/TRA_GlobalConfig.conf")
	{
		if (m_Config != null)
			return m_Config;
		
		Resource resource = Resource.Load(ConfigRes);
		
		if (!resource.IsValid())
			return null;
		
		BaseContainer bCont = resource.GetResource().ToBaseContainer();
		
		Managed mngd = BaseContainerTools.CreateInstanceFromContainer(bCont);
		
		TRA_GlobalConfig test = TRA_GlobalConfig.Cast(mngd);
		
		m_Config = test;
		return test;
	}
}