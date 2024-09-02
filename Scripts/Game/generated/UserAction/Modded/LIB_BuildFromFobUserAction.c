class LIB_BuildFromFobUserAction : ScriptedUserAction
{
	protected SCR_ChimeraCharacter m_CharacterOwner;
	
	protected TRA_GlobalConfig m_config = TRA_GlobalConfig.GetConfig();
	
	protected CameraBase m_CharacterCamera;
	protected IEntity m_PreviewEntity;
	protected bool m_InHand = false;
	protected bool m_bActivated = false;

	protected LIB_BuildStructureComponent placingStructureComp;
	
	//------------------------------------------------------------------------------------------------
	// {4BE4C27399CF3B00}Prefabs/Structures/Military/Bunkers/ShelterMilitary_E_01/ShelterMilitary_E_01.et
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		World world = GetOwner().GetWorld();
		PlayerController pController = GetGame().GetPlayerController();;
		LIB_BuildingMenuManager buildingManager = LIB_BuildingMenuManager.Cast(pController.FindComponent(LIB_BuildingMenuManager));
		
		
		buildingManager.OpenBuildMenuUI("BuildMenuRoot", "{EE7678AD175079FA}UI/layouts/BuildingUI/BuildMenu.layout");
		
		/*
		RandomGenerator randomGenerator = new RandomGenerator();
		vector teleportPosition = randomGenerator.GenerateRandomPointInRadius(m_iSpawnMinDist, m_iSpawnMaxDist, m_vTeleportDestination);
		pUserEntity.SetOrigin(teleportPosition);
		*/
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		return true;
	}

	//------------------------------------------------------------------------------------------------
	override bool HasLocalEffectOnlyScript()
	{
		return true;
	}
}