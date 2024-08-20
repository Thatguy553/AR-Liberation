class LIB_AiManagerClass
{
	static ref LIB_AiManagerClass m_sInstance;
	
	//------------------------------------------------------------------------------------------------
	static LIB_AiManagerClass GetInstance()
	{
		if (!m_sInstance)
		{
			m_sInstance = new LIB_AiManagerClass();
		}
		return m_sInstance;
	}
	
	//------------------------------------------------------------------------------------------------
	void LIB_AiManagerClass()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity AiSpawner(ResourceName spawnGroup, vector spawnPosition, vector waypointPosition, out SCR_AIGroup outGroup, LIB_ObjectiveComponent caller = null, string waypointType = "")
	{
		Resource resource = GenerateAndValidateResource(spawnGroup);
		
		if (!resource)
 		{
 			Print(("[AI Spawner] Unable able to load resource for the spawn group: " + spawnGroup), LogLevel.ERROR);
 			return null;
 		}
 		
 		// Generate the spawn parameters and spawn the group
		SCR_AIGroup SpawnedPrefab = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(spawnPosition)));
 		
		if (caller != null)
		{
			SpawnedPrefab.GetOnAgentAdded().Insert(caller.OnAgentAdded);
		}
		
 		if (!SpawnedPrefab)
 		{
 			Print("[AI Spawner] Unable to spawn group!", LogLevel.ERROR);
 			return null;
 		}
		
 		// Create a waypoint for this group
 		CreateWaypoint(SpawnedPrefab, waypointType, waypointPosition);
		
		outGroup = SpawnedPrefab;
		
		return SpawnedPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	IEntity SpawnMannedVeh(ResourceName veh, string crew, vector spawnPos, out SCR_AIGroup outGrp, LIB_ObjectiveComponent caller = null)
	{
		Resource resource = GenerateAndValidateResource(veh);
		
		if (!resource)
 		{
 			Print(("[AI Spawner] Unable able to load resource for the spawn group: " + veh), LogLevel.ERROR);
 			return null;
 		}
 		
		float areaRadius = 30;
		
		if (caller != null)
		{
			areaRadius = caller.GetObjSpwnRadius();
		}
		
		vector newSpawnPos;
		SCR_WorldTools.FindEmptyTerrainPosition(newSpawnPos, spawnPos, areaRadius, 2, 4);
		
 		// Generate the spawn parameters and spawn the group
		IEntity SpawnedPrefab = GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(newSpawnPos));
		
 		if (!SpawnedPrefab)
 		{
 			Print("[AI Spawner] Unable to spawn group!", LogLevel.ERROR);
 			return null;
 		}
		
		return SpawnedPrefab;
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateWaypoint(SCR_AIGroup group, string waypointType, vector waypointPosition)
 	{
 		// Generate the resource 
 		Resource resource = GenerateAndValidateResource(waypointType);
 		
 		if (!resource)
 		{
 			Print(("[Create Waypoint] Unable able to load resource for the waypoint: " + waypointType), LogLevel.ERROR);
 			return;
 		}
 		
 		// Generate the spawn parameters and create the waypoint 
 		AIWaypoint waypoint = AIWaypoint.Cast(GetGame().SpawnEntityPrefab(resource, null, GenerateSpawnParameters(waypointPosition)));
 		
 		if (!waypoint)
	 	{
 			Print("[Create Waypoint] Unable to create waypoint!", LogLevel.ERROR);
 			return;
 		}
 		
 		// Assign the waypoint to the group 
 		group.AddWaypoint(waypoint);
	}
	
	//------------------------------------------------------------------------------------------------
	Resource GenerateAndValidateResource(string resourceToLoad)
 	{
 		// Load the resource 
 		Resource resource = Resource.Load(resourceToLoad);
 		
 		// Validate the prefab and show an appropriate error if invalid 
 		if (!resource.IsValid())
 		{
 			Print(("[Generate And Validate Resource] Resource is invalid: " + resourceToLoad), LogLevel.ERROR);
 			return null;
 		}
 		
 		// Return the resource 
 		return resource;
 	}
 	
	//------------------------------------------------------------------------------------------------
 	EntitySpawnParams GenerateSpawnParameters(vector position)
 	{
 		// Create a new set of spawn parameters 
 		EntitySpawnParams params = EntitySpawnParams();
 		params.TransformMode = ETransformMode.WORLD;
 	
 		// Assign the position to those parameters 
 		params.Transform[3] = position;
 		
 		// Return this set of spawn parameters
 		return params;
 	}
	
	//------------------------------------------------------------------------------------------------
	
}
