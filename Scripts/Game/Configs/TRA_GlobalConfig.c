[BaseContainerProps(configRoot: true)]
class TRA_GlobalConfig
{
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
	// Fob related data
	[Attribute(defvalue: "{4BE4C27399CF3B00}Prefabs/Structures/Military/Bunkers/ShelterMilitary_E_01/ShelterMilitary_E_01.et", desc: "Blufor FOB building prefab", category: "Blufor Fob Data")]
	ResourceName m_BFobPrefab;
	
	[Attribute(defvalue: "5", desc: "Distance from the player the preview/obj will be.", category: "Fob Data")]
	float m_BPreviewDist;
	
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