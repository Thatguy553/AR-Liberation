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

	[Attribute(category: "Objective Settings", desc: "Type of Obj", UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(LIB_EObjType))]
	protected LIB_EObjType m_ObjType;
	
	[Attribute(category: "Objective Settings", defvalue: "850", desc: "A Radius in meters for the objective to detect players in.")]
	protected float m_ObjActRadius;
	
	[Attribute(category: "Objective Settings", defvalue: "600", desc: "A Radius in meters for the objective to spawn ai and objects in.")]
	protected float m_ObjSpwnRadius;

	[Attribute(category: "Objective Settings", defvalue: "USSR", desc: "The faction key for this objective to start with.")]
	protected FactionKey m_ObjFaction;

	[Attribute(category: "Objective Settings", defvalue: "Obj Name", desc: "The name for this objective to use.")]
	protected string m_ObjName;
	
	[Attribute(category: "Objective Settings", defvalue: "30", desc: "The amount of ai infantry that should spawn at this objective")]
	protected int m_ObjAiCount;
	
	[Attribute(category: "Objective Settings", defvalue: "3", desc: "The max amount of manned vehicles that can spawn at this objective")]
	protected int m_ObjMaxVeh;
	
	[Attribute(category: "Objective Settings", defvalue: "30", desc: "The amount of time this objective must be held in order to be captured in seconds.")]
	protected int m_ObjCapTime;
	protected int m_ObjCapCurTime = 0;
	protected int m_ObjCapStartTime = 0;
	
	protected ref array<ref EntityID> m_Houses = {};
	protected ref array<IEntity> m_Units = {};
	protected ref array<IEntity> m_PopulatedBuildings = {};
	protected LIB_AiManagerClass m_AiManager;
	protected LIB_TownManagerComponent m_ObjManager;
	
	// Should be [ImgWidget, TextWidget]
	protected array<Widget> m_MarkerWidgets;
	
	protected IEntity m_ObjectiveEnt;
	protected TRA_GlobalConfig m_sConfig;
	
	// Total units in the zone on each side.
	protected int m_OpforTotal = 0;
	protected int m_BluforTotal = 0;
	
	// Getters/Setters
	//------------------------------------------------------------------------------------------------
	/*!
		Returns the type of objective. See LIB_EObjType for the currently possible types.
	*/
	LIB_EObjType GetObjType()
	{
		return m_ObjType;
	}

	//------------------------------------------------------------------------------------------------
	/*!
		Sets the current type of objective. See LIB_EObjType for the currently possible types.
	*/
	void SetObjType(LIB_EObjType type)
	{
		m_ObjType = type;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Returns [ImageWidget, TextWidget]
	*/
	array<Widget> GetMarkerWidgets()
	{
		return m_MarkerWidgets;
	}

	//------------------------------------------------------------------------------------------------
	/*!
		sets the array of map marker widgets for this objective.
		Intended for use in SCR_TRA_MapUnitMarkerUI.
	*/
	void SetMarkerWidgets(ImageWidget imgWidget, TextWidget txtWidget)
	{
		m_MarkerWidgets.Insert(imgWidget);
		m_MarkerWidgets.Insert(txtWidget);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	float GetObjSpwnRadius()
	{
		return m_ObjSpwnRadius;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void SetObjSpwnRadius(float radius)
	{
		m_ObjSpwnRadius = radius;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	FactionKey GetObjFaction()
	{
		return m_ObjFaction;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void SetObjFaction(FactionKey faction)
	{
		m_ObjFaction = faction;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	string GetObjName()
	{
		return m_ObjName;
	}

	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void SetObjName(string name)
	{
		m_ObjName = name;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	int GetObjAiCount()
	{
		return m_ObjAiCount;
	}

	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void SetObjAiCount(int count)
	{
		m_ObjAiCount = count;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	int GetOpforTotal()
	{
		return m_OpforTotal;
	}

	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void AddOpforTotal(int count)
	{
		m_OpforTotal += count;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	int GetBluforTotal()
	{
		return m_BluforTotal;
	}

	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void SetBluforTotal(int count)
	{
		m_BluforTotal = count;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void AddBluforTotal(int count)
	{
		m_BluforTotal += count;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Should return all units from m_Units array, however I *think* it only returns the living units. Need to test as of 0.1.04.
	*/
	array<IEntity> GetObjUnits()
	{
		return m_Units;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void AddObjUnits(IEntity unit)
	{
		m_Units.Insert(unit);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
	
	*/
	void AddObjUnits(array<IEntity> units)
	{
		m_Units.InsertAll(units);
	}
	//------------------------------------------------------------------------------------------------
	// Event Methods
	protected override void EOnFrame(IEntity owner, float timeSlice)
	{
		bool capturing = IsCapturing();
		if (m_ObjFaction == "USSR" && capturing && m_BluforTotal > 0)
		{
			m_ObjCapCurTime = System.GetTickCount();
			//Print(m_ObjCapCurTime);
			if (m_ObjCapStartTime == 0)
			{
				m_ObjCapStartTime = System.GetTickCount();
			}
			//Print(m_ObjCapStartTime);
			
			//Print(m_ObjCapCurTime - m_ObjCapStartTime);
			// Only runs if it has been x seconds since capturing started
			if ((m_ObjCapCurTime - m_ObjCapStartTime) >= m_ObjCapTime)
			{
				//Print("Captured");
				Capture();
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	protected override void EOnInit(IEntity owner)
	{
		m_ObjManager = LIB_TownManagerComponent.GetInstance();
		
		// Obj manager now only stores, no longer creates obj markers, still does nothing with them as of 0.1.04
		m_ObjManager.AddObjMarker(m_MarkerWidgets);
		
		m_AiManager = LIB_AiManagerClass.GetInstance();
		
		// Convert m_ObjCapTime from seconds to ms
		m_ObjCapTime = m_ObjCapTime * 1000;
		
		// This is the object entity all components are attached too.
		m_ObjectiveEnt = owner;
		
		// Custom Config of units
		m_sConfig = TRA_GlobalConfig.GetConfig();
		
		// Assign this objective to whichever array it belongs in the obj manager
		switch(m_ObjFaction)
		{
			case "US":
				m_ObjManager.AddFriendlyObjArray(m_ObjectiveEnt);
				break;
			case "USSR":
				m_ObjManager.AddHostileObjArray(m_ObjectiveEnt);
				break;
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		Print(owner.GetName(), LogLevel.NORMAL);
		SetEventMask(owner, EntityEvent.FRAME | EntityEvent.INIT);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Gets a randomized house from this objective, returns the IEntity of the house.
	*/
	IEntity GetRandomHouse()
	{
		m_Houses = new array<ref EntityID>;
		
		GetGame().GetWorld().QueryEntitiesBySphere(m_ObjectiveEnt.GetOrigin(), m_ObjSpwnRadius, CheckHouseAddToArray, FilterHouseEntities, EQueryEntitiesFlags.STATIC);
		
		return GetGame().GetWorld().FindEntityByID(m_Houses.GetRandomElement());
	}
	
	
	/*!
	
	*/
	protected bool CheckHouseAddToArray(IEntity entity)
	{
		m_Houses.Insert(entity.GetID());
				
		return true;
	}
	
	
	/*!
	
	*/
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
	/*!
		Objective Activation Methods
	*/
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
			IEntity spawnBuilding = GetRandomHouse();
			if (m_PopulatedBuildings.Find(spawnBuilding) > -1)
			{
				continue;
			}
			
			vector spawnPosition = spawnBuilding.GetOrigin();
			m_PopulatedBuildings.Insert(spawnBuilding);
			
			string waypointType = "{93291E72AC23930F}Prefabs/AI/Waypoints/AIWaypoint_Defend.et";
			SCR_AIGroup group;
			ResourceName groupToSpawn = m_sConfig.m_OInfGroups.GetRandomElement();
			
			
			m_Units.Insert(m_AiManager.AiSpawner(groupToSpawn, spawnPosition, waypointType, spawnPosition, group, this));
			
			AiCount += group.GetNumberOfMembersToSpawn();
		}
		m_OpforTotal += AiCount;
		
		for (int i; AiCount <= m_ObjMaxVeh; i++)
		{
			IEntity spawnBuilding = GetRandomHouse();
			vector spawnPosition = spawnBuilding.GetOrigin();
			
			m_AiManager.SpawnMannedVeh(m_sConfig.m_OVehGroups.GetRandomElement(), m_sConfig.m_OLightFireTeam, spawnPosition);
			
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
		//Print("Checking if capturing.");
		//Print(m_BluforTotal);
		//Print(m_OpforTotal);
		if (m_BluforTotal > m_OpforTotal)
		{
			//Print("Capturing.");
			return true;
		}
		//Print("Not Capturing.");
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	// When activated, assumes all conditions are met.
	// Should probably change naming from "Blufor/Opfor" to something more generic, i.e. friendly/enemy
	void Capture()
	{
		// Switch map marker color
		//LIB_ScenarioFrameworkSlotMarker markerComp = LIB_ScenarioFrameworkSlotMarker.Cast(m_MarkerEnt.FindComponent(LIB_ScenarioFrameworkSlotMarker));
		//Print("Setting color to blue");
		//markerComp.SetMapMarkerColor(LIB_EScenarioFrameworkMarkerCustomColor.BLUFOR);
		ImageWidget imgWidget = ImageWidget.Cast(m_MarkerWidgets[0]);
		imgWidget.SetColor(Color.Blue);
		
		// Create a marker to point out any remaining units from this objective
		foreach(IEntity unit : m_Units)
		{
			// Attach marker, probably check out mod Unit Map Markers V2
		}
		
		// Announce objective capture
		m_ObjManager.RemHostileObj(m_ObjectiveEnt);
		m_ObjManager.AddFriendlyObjArray(m_ObjectiveEnt);
		m_ObjManager.TellObjCaptured(m_ObjectiveEnt);
	}
}