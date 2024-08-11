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
	array<ResourceName> m_OInfGroups;
	
	[Attribute(defvalue: "", desc: "Opfor BTR", category: "Vehicle Prefabs", params: "et")]
	ResourceName m_OBtr;
	
	[Attribute(defvalue: "", desc: "Opfor UAZ", category: "Vehicle Prefabs", params: "et")]
	ResourceName m_OUaz;
	
	[Attribute(defvalue: "", desc: "Opfor Vehicle Prefabs Array", category: "Vehicle Prefabs Array")]
	array<ResourceName> m_OVehGroups;
	
	//------------------------------------------------------------------------------------------------
	static TRA_GlobalConfig  m_Config;
	
	static TRA_GlobalConfig GetConfig(ResourceName ConfigRes = "{80A853CA085D4CD2}Configs/Global/TRA_GlobalConfig.conf")
	{
		Print("m_sConfigRes");
		Print(ConfigRes);
		if (m_Config != null)
			return m_Config;
		
		Print("m_sConfig");
		Print(m_Config);
		Resource resource = Resource.Load(ConfigRes);
		
		Print("m_sresource");
		Print(resource);
		
		Print(resource.IsValid());
		if (!resource.IsValid())
			return null;
		TRA_GlobalConfig test = TRA_GlobalConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(resource.GetResource().ToBaseContainer()));
		Print(test);
		m_Config = test;
		return test;
	}
}