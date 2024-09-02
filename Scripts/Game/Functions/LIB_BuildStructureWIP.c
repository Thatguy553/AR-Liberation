class LIB_BuildStructureWIP
{
	protected BaseGameMode m_gameMode;
	protected LIB_FobManagerComponent m_FobComp;

	//------------------------------------------------------------------------------------------------
	void build(ResourceName resourcename, vector mat[4], bool isFOB)
	{
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = mat;
		
		IEntity wipStructure = null;
		Resource resource;
		int index = -1;

		
		resource = Resource.Load(resourcename);

		// Step 2: If pos inside base area (base area unions not handled, add structure to first possible base)
		bool inside = false;
		/*for (int i = 0; i < basecount; i++)
		{
			if (!inside)
			{
				float distance = m_FobComp.getAreaDistances(mat[3], m_FobComp.getBase(factionkey, i).getBasePos());

				if (distance <= SCR_CTI_Constants.BASERADIUS)
				{
					wipStructure = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
					wipStructure.Update();
					inside = true;
				}
			}
		}*/

		wipStructure = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
		wipStructure.Update();


		if (wipStructure)
		{

			// Store wip structure IDs for searching
			RplComponent rplComp = RplComponent.Cast(wipStructure.FindComponent(RplComponent));
			RplId rplid = rplComp.Id();
			
			if (isFOB)
				m_FobComp.AddFobRplId(rplid);

			
			
			// Pay the cost
			//int cost = factoryData.getPrice();
			//m_gameMode.changeCommanderFunds(factionkey, -cost);

			// Parts SnapToGround
			BaseWorld world = GetGame().GetWorld();
			IEntity child = wipStructure.GetChildren();
			while (child)
			{
				vector orig = child.GetOrigin();
				float y = SCR_TerrainHelper.GetTerrainY(orig, world);
				if (orig[1] < y + 2)
				{
					orig[1] = y;
					child.SetOrigin(orig);
					child.Update();
				}

				child = child.GetSibling();
			}

			// Navmesh regenerate
			SCR_AIWorld aiWorld = SCR_AIWorld.Cast(GetGame().GetAIWorld());
			aiWorld.RequestNavmeshRebuildEntity(wipStructure);
		} else {
			Print("LIB :: Something went wrong, wipStructure is null");
		}
	}

	//------------------------------------------------------------------------------------------------
	void LIB_BuildStructureWIP()
	{
		m_gameMode = GetGame().GetGameMode();
		m_FobComp = LIB_FobManagerComponent.Cast(m_gameMode.FindComponent(LIB_FobManagerComponent));
	}

	//------------------------------------------------------------------------------------------------
	void ~LIB_BuildStructureWIP()
	{
	}
};