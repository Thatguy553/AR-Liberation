class LIB_ObjectiveActionInit : SCR_ScenarioFrameworkActionBase
{
	// Opfor Group Prefabs
	protected string m_OFireTeam = "{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et"; // 4x, TL, AR, GL, R
	protected string m_OLightFireTeam = "{657590C1EC9E27D3}Prefabs/Groups/OPFOR/Group_USSR_LightFireTeam.et"; // 4x, TL, R, GL, R
	protected string m_OMgTeam = "{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et"; // 2x, MG, AMG
	protected string m_OManeuverGrp = "{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et"; // 2x, TL, R
	protected string m_OMedics = "{D815658156080328}Prefabs/Groups/OPFOR/Group_USSR_MedicalSection.et"; // 2x, M, M
	protected string m_ORifleSqd = "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et"; // 6x, TL, AR, AT, AAT, R, R
	protected string m_OAtTeam = "{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et"; // 4x, TL, AT, AT, AAT
	protected string m_OSuppTeam = "{1C0502B5729E7231}Prefabs/Groups/OPFOR/Group_USSR_Team_Suppress.et"; // 4x, TL, AR, AR, GL
	
	

	override void OnActivate(IEntity object)
	{
	}
}