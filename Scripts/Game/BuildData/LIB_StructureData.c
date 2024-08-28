class LIB_StructureData
{
	protected ResourceName m_resourceName;		// Resource name
	//protected FactionKey m_factionKey;			// Facton key
	protected EntityID m_groupId;				// Requester groupId
	protected RplId m_StructRplId;				// Factory RplId
	protected vector m_mat[4];					// Early spawn area
	protected int m_playerId;					// Requester PlayerId
	protected int m_buildTime;					// Needed build time
	//protected CTI_PurchaseInfos m_purchaseInfo;	// Purchase infos
	//protected int m_requestTime;				// Request time

	//------------------------------------------------------------------------------------------------
	void setData(ResourceName newResourceName, EntityID newGroupId, RplId newStructureRplId, vector newMat[4], int newPlayerId, int newBuildTime)
	{
		m_resourceName = newResourceName;
		//m_factionKey = newFactionKey;
		m_groupId = newGroupId;
		m_StructRplId = newStructureRplId;
		m_mat = newMat;
		m_playerId = newPlayerId;
		m_buildTime = newBuildTime;
		//m_purchaseInfo = newPurchaseInfo;

		//SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		//m_requestTime = gameMode.GetElapsedTime();
	}
	
	//------------------------------------------------------------------------------------------------
	void findBuildDataBy()
	{
		// TODO request time use for ID
	}
	
	//------------------------------------------------------------------------------------------------
	void getData(out ResourceName resourceName, out EntityID groupId, out RplId structureRplId, out vector mat[4], out int playerId, out int buildTime)
	{
		resourceName = m_resourceName;
		//factionKey = m_factionKey;
		groupId = m_groupId;
		structureRplId = m_StructRplId;
		mat = m_mat;
		playerId = m_playerId;
		buildTime = m_buildTime;
		//purchaseInfo = m_purchaseInfo;
		//requestTime = m_requestTime;
	}

	//------------------------------------------------------------------------------------------------
	void LIB_StructureData()
	{
	}

	//------------------------------------------------------------------------------------------------
	void ~LIB_StructureData()
	{
	}
};