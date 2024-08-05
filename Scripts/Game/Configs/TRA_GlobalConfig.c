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
}