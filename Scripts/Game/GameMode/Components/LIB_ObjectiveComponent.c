[ComponentEditorProps(category: "Objective/Component", description: "Component to attach to objective areas, should be updated to contain the objective information")]
class LIB_ObjectiveComponentClass : ScriptComponentClass
{
}

class LIB_ObjectiveComponent : ScriptComponent
{
	/*
		Objective Type:
		
		TOWN, // 0
		CAPITAL, // 1
		MILITARY, // 2
		RADIO_TOWER, // 3
		FACTORY // 4
	*/

	[Attribute("Type of Obj", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(LIB_EObjType))]
	protected LIB_EObjType m_ObjType;
	
	[Attribute("Objective Activation Radius", defvalue: "850", desc: "A Radius in meters for the objective to detect players in.")]
	protected float m_ObjActRadius;
	
	[Attribute("Objective Spawn Radius", defvalue: "600", desc: "A Radius in meters for the objective to spawn ai and objects in.")]
	protected float m_ObjSpwnRadius;

	[Attribute("Objective Faction", defvalue: "USSR", desc: "The faction key for this objective to start with.")]
	protected FactionKey m_ObjFaction;

	[Attribute("Objective Name", defvalue: "Obj Name", desc: "The name for this objective to use.")]
	protected string m_ObjName;
	
	[Attribute("Ai Count", defvalue: "30", desc: "The amount of ai infantry that should spawn at this objective")]
	protected int m_ObjAiCount;
	
	[Attribute("Max Vehicle Count", defvalue: "3", desc: "The max amount of manned vehicles that can spawn at this objective")]
	protected int m_ObjMaxVeh;
	
	[Attribute("Obj Capture Time", defvalue: "30", desc: "The amount of time this objective must be held in order to be captured in seconds.")]
	protected int m_ObjCapTime;
	protected int m_ObjCapCurTime = 0;
	protected int m_ObjCapStartTime = 0;
	
	protected ref array<ref EntityID> m_Houses = {};
	protected ref array<IEntity> m_Units = {};
	protected ref array<IEntity> m_PopulatedBuildings = {};
	protected LIB_AiManagerClass m_AiManager;
	protected LIB_TownManagerComponent m_ObjManager;
	protected IEntity m_MarkerEnt;
	
	// Total units in the zone on each side.
	protected int m_OpforTotal = 0;
	protected int m_BluforTotal = 0;
	
	//------------------------------------------------------------------------------------------------
	// Probably wasting resources? should put somewhere else later, not in every instance of Obj Component
	// Opfor Group Prefabs
	protected ResourceName m_OFireTeam = "{30ED11AA4F0D41E5}Prefabs/Groups/OPFOR/Group_USSR_FireGroup.et"; // 4x, TL, AR, GL, R
	protected ResourceName m_OLightFireTeam = "{657590C1EC9E27D3}Prefabs/Groups/OPFOR/Group_USSR_LightFireTeam.et"; // 4x, TL, R, GL, R
	protected ResourceName m_OMgTeam = "{A2F75E45C66B1C0A}Prefabs/Groups/OPFOR/Group_USSR_MachineGunTeam.et"; // 2x, MG, AMG
	protected ResourceName m_OManeuverGrp = "{1A5F0D93609DA5DA}Prefabs/Groups/OPFOR/Group_USSR_ManeuverGroup.et"; // 2x, TL, R
	protected ResourceName m_OMedics = "{D815658156080328}Prefabs/Groups/OPFOR/Group_USSR_MedicalSection.et"; // 2x, M, M
	protected ResourceName m_ORifleSqd = "{E552DABF3636C2AD}Prefabs/Groups/OPFOR/Group_USSR_RifleSquad.et"; // 6x, TL, AR, AT, AAT, R, R
	protected ResourceName m_OAtTeam = "{96BAB56E6558788E}Prefabs/Groups/OPFOR/Group_USSR_Team_AT.et"; // 4x, TL, AT, AT, AAT
	protected ResourceName m_OSuppTeam = "{1C0502B5729E7231}Prefabs/Groups/OPFOR/Group_USSR_Team_Suppress.et"; // 4x, TL, AR, AR, GL
	protected ref array<ResourceName> m_OInfGroups = {};
	
	protected ResourceName m_OBTR = "{C012BB3488BEA0C2}Prefabs/Vehicles/Wheeled/BTR70/BTR70.et";
	protected ResourceName m_OUAZ = "{0B4DEA8078B78A9B}Prefabs/Vehicles/Wheeled/UAZ469/UAZ469_PKM.et";
	protected ref array<ResourceName> m_OVehGroups = {};
	
	// Getters/Setters
	//------------------------------------------------------------------------------------------------
	LIB_EObjType GetObjType()
	{
		return m_ObjType;
	}

	//------------------------------------------------------------------------------------------------
	void SetObjType(LIB_EObjType type)
	{
		m_ObjType = type;
	}	
	
	//------------------------------------------------------------------------------------------------
	float GetObjSpwnRadius()
	{
		return m_ObjSpwnRadius;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetObjSpwnRadius(float radius)
	{
		m_ObjSpwnRadius = radius;
	}
	
	//------------------------------------------------------------------------------------------------
	string GetObjFaction()
	{
		return m_ObjFaction;
	}
	
	//------------------------------------------------------------------------------------------------
	void SetObjFaction(string faction)
	{
		m_ObjFaction = faction;
	}
	
	//------------------------------------------------------------------------------------------------
	string GetObjName()
	{
		return m_ObjName;
	}

	//------------------------------------------------------------------------------------------------
	void SetObjName(string name)
	{
		m_ObjName = name;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetObjAiCount()
	{
		return m_ObjAiCount;
	}

	//------------------------------------------------------------------------------------------------
	void SetObjAiCount(int count)
	{
		m_ObjAiCount = count;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetOpforTotal()
	{
		return m_OpforTotal;
	}

	//------------------------------------------------------------------------------------------------
	void AddOpforTotal(int count)
	{
		m_OpforTotal += count;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetBluforTotal()
	{
		return m_BluforTotal;
	}

	//------------------------------------------------------------------------------------------------
	void SetBluforTotal(int count)
	{
		m_BluforTotal = count;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddBluforTotal(int count)
	{
		m_BluforTotal += count;
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetObjUnits()
	{
		return m_Units;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddObjUnits(IEntity unit)
	{
		m_Units.Insert(unit);
	}
	
	//------------------------------------------------------------------------------------------------
	void AddObjUnits(array<IEntity> units)
	{
		m_Units.InsertAll(units);
	}
	
	//------------------------------------------------------------------------------------------------
	
	// Event Methods
	//------------------------------------------------------------------------------------------------
	protected override void EOnFrame(IEntity owner, float timeSlice)
	{
		bool capturing = IsCapturing();
		if (m_ObjFaction == "USSR" && capturing && m_BluforTotal > 0)
		{
			m_ObjCapCurTime = System.GetTickCount();
			Print(m_ObjCapCurTime);
			if (m_ObjCapStartTime == 0)
			{
				m_ObjCapStartTime = System.GetTickCount();
			}
			Print(m_ObjCapStartTime);
			
			Print(m_ObjCapCurTime - m_ObjCapStartTime);
			// Only runs if it has been x seconds since capturing started
			if ((m_ObjCapCurTime - m_ObjCapStartTime) >= m_ObjCapTime)
			{
				Print("Captured");
				Capture();
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void EOnInit(IEntity owner)
	{
		m_ObjManager = LIB_TownManagerComponent.GetInstance();
	
		// Makes the objective manager create and store the marker for this objective. Manager does nothing with this marker as of 0.1.0
		m_MarkerEnt = m_ObjManager.CreateObjMarker(owner, this);
		m_ObjManager.AddObjMarker(m_MarkerEnt);
		
		m_AiManager = LIB_AiManagerClass.GetInstance();
		
		// Convert m_ObjCapTime from seconds to ms
		m_ObjCapTime = m_ObjCapTime * 1000;
		
		// To be moved to a custom config eventually
		m_OInfGroups.Insert(m_OFireTeam);
		m_OInfGroups.Insert(m_OLightFireTeam);
		m_OInfGroups.Insert(m_OMgTeam);
		m_OInfGroups.Insert(m_OManeuverGrp);
		m_OInfGroups.Insert(m_OMedics);
		m_OInfGroups.Insert(m_ORifleSqd);
		m_OInfGroups.Insert(m_OAtTeam);
		m_OInfGroups.Insert(m_OSuppTeam);
		
		m_OVehGroups.Insert(m_OBTR);
		m_OVehGroups.Insert(m_OUAZ);
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		Print(owner.GetName(), LogLevel.NORMAL);
		SetEventMask(owner, EntityEvent.FRAME | EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	// Town Building related functions
	IEntity GetRandomHouse(IEntity owner)
	{
		m_Houses = new array<ref EntityID>;
		
		GetGame().GetWorld().QueryEntitiesBySphere(owner.GetOrigin(), m_ObjSpwnRadius, CheckHouseAddToArray, FilterHouseEntities, EQueryEntitiesFlags.STATIC);
		
		return GetGame().GetWorld().FindEntityByID(m_Houses.GetRandomElement());
	}
	
	protected bool CheckHouseAddToArray(IEntity entity)
	{
		m_Houses.Insert(entity.GetID());
				
		return true;
	}
	
	protected bool FilterHouseEntities(IEntity entity) 
	{
		if(entity.ClassName() == "SCR_DestructibleBuildingEntity"){
			VObject mesh = entity.GetVObject();
			
			if(mesh){
				string res = mesh.GetResourceName();
				if(res.IndexOf("/Military/") > -1) return false;
				
				if(res.IndexOf("/Houses/") > -1){
					if(res.IndexOf("/Shed/") > -1) return false;
					if(res.IndexOf("/Garage/") > -1) return false;
					if(res.IndexOf("/HouseAddon/") > -1) return false;
					return true;
				}
					
			}
		}
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	// Objective Activation Methods
	void ActivateObj(IEntity obj)
	{
		switch(m_ObjType)
		{
			case LIB_EObjType.TOWN:
				InitTown(obj);
				break;
			case LIB_EObjType.FACTORY:
				InitFactory();
				break; 
			case LIB_EObjType.CAPITAL:
				InitCapital();
				break; 
			case LIB_EObjType.MILITARY:
				InitMilitary();
				break;
			case LIB_EObjType.RADIO_TOWER:
				InitRadioTower();
				break; 
		}
	}
	
	protected void InitTown(IEntity obj)
	{
		int AiCount = 0;
		
		for (int i; AiCount <= m_ObjAiCount; i++)
		{
			IEntity spawnBuilding = GetRandomHouse(obj);
			if (m_PopulatedBuildings.Find(spawnBuilding) > -1)
			{
				continue;
			}
			
			vector spawnPosition = spawnBuilding.GetOrigin();
			m_PopulatedBuildings.Insert(spawnBuilding);
			
			string waypointType = "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et";
			SCR_AIGroup group;
			ResourceName groupToSpawn = m_OInfGroups.GetRandomElement();
			
			
			m_Units.Insert(m_AiManager.AiSpawner(groupToSpawn, spawnPosition, waypointType, spawnPosition, group, this));
			
			AiCount += group.GetNumberOfMembersToSpawn();
		}
		m_OpforTotal += AiCount;
		
		for (int i; AiCount <= m_ObjMaxVeh; i++)
		{
			IEntity spawnBuilding = GetRandomHouse(obj);
			vector spawnPosition = spawnBuilding.GetOrigin();
			
			m_AiManager.SpawnMannedVeh(m_OVehGroups.GetRandomElement(), m_OLightFireTeam, spawnPosition);
			
			// hard coded for now, m_OLightFireTeam has 4 units.
			m_BluforTotal += 4;
		}
	}
	
	protected void InitFactory()
	{
	
	}
	
	protected void InitCapital()
	{
	
	}
	
	protected void InitMilitary()
	{
	
	}
	
	protected void InitRadioTower()
	{
	
	}
	
	void DeactivateObj()
	{
		foreach (IEntity unit : m_Units)
		{
			Print(unit);
			SCR_EntityHelper.DeleteEntityAndChildren(unit);	
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void OnAgentAdded(AIAgent agent)
	{
		IEntity ent = agent.GetControlledEntity();
		
		EventHandlerManagerComponent eventHandler = EventHandlerManagerComponent.Cast(ent.FindComponent(EventHandlerManagerComponent));
        if (!eventHandler)
            return;
        
        if (Replication.IsServer())
            eventHandler.RegisterScriptHandler("OnDestroyed", this, OnDestroyed);
	}
	
	//------------------------------------------------------------------------------------------------
	void OnDestroyed(IEntity ent)
    {
        Print("Entity Killed");
        Print(ent);
		
		m_OpforTotal -= 1;
    }
	
	//------------------------------------------------------------------------------------------------
	bool IsCapturing()
	{
		Print("Checking if capturing.");
		Print(m_BluforTotal);
		Print(m_OpforTotal);
		if (m_BluforTotal > m_OpforTotal)
		{
			Print("Capturing.");
			return true;
		}
		Print("Not Capturing.");
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	// When activated, assumes all conditions are met.
	// Should probably change naming from "Blufor/Opfor" to something more generic, i.e. friendly/enemy
	void Capture()
	{
		// Switch map marker color
		LIB_ScenarioFrameworkSlotMarker markerComp = LIB_ScenarioFrameworkSlotMarker.Cast(m_MarkerEnt.FindComponent(LIB_ScenarioFrameworkSlotMarker));
		markerComp.SetMapMarkerColor(LIB_EScenarioFrameworkMarkerCustomColor.BLUFOR);
		
		// Create a marker to point out any remaining units from this objective
		
		// Announce objective capture
	}
}