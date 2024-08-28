class LIB_BuildFobUserAction : ScriptedUserAction
{
	[Attribute(defvalue: "0 0 0", uiwidget: UIWidgets.Coords, desc: "Teleport destination")]
	protected vector m_vTeleportDestination;

	[Attribute(defvalue: "2", uiwidget: UIWidgets.EditBox, desc: "Spawn min. distance")]
	protected int m_iSpawnMinDist;

	[Attribute(defvalue: "4", uiwidget: UIWidgets.EditBox, desc: "Spawn max. distance")]
	protected int m_iSpawnMaxDist;
	
	protected SCR_ChimeraCharacter m_CharacterOwner;
	
	protected TRA_GlobalConfig m_config = TRA_GlobalConfig.GetConfig();
	
	protected ResourceName m_sFobPrefabName = m_config.m_BFobPrefab;
	
	protected float m_PlacementDistance = m_config.m_BPreviewDist;
	
	protected CameraBase m_CharacterCamera;
	protected IEntity m_PreviewEntity;
	protected bool m_InHand = false;
	protected bool m_bActivated = false;

	protected LIB_BuildStructureComponent placingStructureComp;
	
	//------------------------------------------------------------------------------------------------
	// {4BE4C27399CF3B00}Prefabs/Structures/Military/Bunkers/ShelterMilitary_E_01/ShelterMilitary_E_01.et
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		PlayerController plyr = GetGame().GetPlayerController();
		placingStructureComp = LIB_BuildStructureComponent.Cast(plyr.FindComponent(LIB_BuildStructureComponent));
		
		placingStructureComp.createStructurePreview("{4BE4C27399CF3B00}Prefabs/Structures/Military/Bunkers/ShelterMilitary_E_01/ShelterMilitary_E_01.et", 10, 10, true);
		
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