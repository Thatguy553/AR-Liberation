class LIB_TownManagerComponentClass : ScriptComponentClass
{
};

class LIB_TownManagerComponent : ScriptComponent
{
	protected static LIB_TownManagerComponent m_sInstance;
	
	protected ref array<IEntity> m_ObjArr = {};
	protected ref array<IEntity> m_ObjMarkerArr = {};
	protected ref array<IEntity> m_ObjActiveArr = {};
	
	//------------------------------------------------------------------------------------------------
	static LIB_TownManagerComponent GetInstance()
	{
		if (!m_sInstance)
	    {
	        m_sInstance = LIB_TownManagerComponent.Cast(GetGame().GetGameMode().FindComponent(LIB_TownManagerComponent));
	        return m_sInstance;
	    }
	    return m_sInstance;
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetObjMarkerArray()
	{
		return m_ObjMarkerArr;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddObjMarker(IEntity ent)
	{
		m_ObjMarkerArr.Insert(ent);
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetObjArray()
	{
		return m_ObjArr;
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetObjActiveArray()
	{
		return m_ObjActiveArr;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddObjActive(IEntity ent)
	{
		m_ObjActiveArr.Insert(ent);
	}
	
	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		// remove if unused
		SetEventMask(owner, EntityEvent.INIT);
	}

	//------------------------------------------------------------------------------------------------
	override void OnDelete(IEntity owner)
	{
		// remove if unused
		if (m_ObjArr) {
			m_ObjArr.RemoveItem(owner)
		}
	}
	//------------------------------------------------------------------------------------------------
	IEntity GetRandObj()
	{
		if (m_ObjArr.IsEmpty()) {
			return null;
		};
		
		return m_ObjArr.GetRandomElement();
	}
	
	array<IEntity> GetObjArr()
	{
		if (m_ObjArr.IsEmpty()) {
			return null;
		};
		
		return m_ObjArr;
	}
	
	IEntity CreateObjMarker(IEntity owner, LIB_ObjectiveComponent comp = null)
	{
		Print("Create Obj Marker");
		
		IEntity slotMarker = GetGame().SpawnEntityPrefab("{E537867C6E760514}Prefabs/ScenarioFramework/Components/Marker Entities/SlotMarker.et", false);
		LIB_ScenarioFrameworkSlotMarker markerComp = LIB_ScenarioFrameworkSlotMarker.Cast(slotMarker.FindComponent(LIB_ScenarioFrameworkSlotMarker));
		LIB_ObjectiveComponent objComp = LIB_ObjectiveComponent.Cast(owner.FindComponent(LIB_ObjectiveComponent));
		
		LIB_EObjType objType = objComp.GetObjType();
		// PrintFormat("Object Component: %1\nObject Type: %2\Marker Component: %3\nSlot Marker: %4\nOwner: %5", objComp, objType, markerComp, slotMarker, owner);
		
		slotMarker.SetOrigin(owner.GetOrigin());
		markerComp.SetScriptSpawned(true);
		
		switch(objType)
		{
			case LIB_EObjType.TOWN:
				Print("Town");
				markerComp.SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom.TOWN);
				break;
			case LIB_EObjType.CAPITAL:
				Print("Capital");
				markerComp.SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom.CAPITAL);
				break;
			case LIB_EObjType.MILITARY:
				Print("Miltiary");
				markerComp.SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom.MILITARY);
				break;
			case LIB_EObjType.FACTORY:
				Print("Factory");
				markerComp.SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom.FUELSTATION);
				break;
			case LIB_EObjType.RADIO_TOWER:
				Print("Radio Tower");
				markerComp.SetMapMarkerIcon(LIB_EScenarioFrameworkMarkerCustom.TRANSMITTER);
				break;
		}
				
		
		markerComp.SetMapMarkerColor(LIB_EScenarioFrameworkMarkerCustomColor.OPFOR);
		markerComp.SetMapMarkerText(owner.GetName());
		markerComp.CreateMapMarker();
		
		return slotMarker;
	}
};
