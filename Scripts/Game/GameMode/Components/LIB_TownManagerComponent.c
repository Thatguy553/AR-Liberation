class LIB_TownManagerComponentClass : ScriptComponentClass
{
};

class LIB_TownManagerComponent : ScriptComponent
{
	protected static LIB_TownManagerComponent m_sInstance;
	
	protected ref array<IEntity> m_HostileObjArr = {};
	protected ref array<IEntity> m_FriendlyObjArr = {};
	protected ref array<array<Widget>> m_ObjMarkerArr = {};
	protected ref array<IEntity> m_ObjActiveArr = {};
	protected int m_TotalObjCount = 0;
	
	//------------------------------------------------------------------------------------------------
	/*!
		Gets the currently active Objective Manager Componenent instance. If one does not exist, creates one.
	*/
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
	/*!
		Returns the total amount of objectives in the world as an int.
	*/
	int GetTotalObjCount()
	{
		return m_TotalObjCount;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Increases the counter for the total amount of objectives in the world.
	*/
	int IncTotalObjCount(int amount)
	{
		m_TotalObjCount += amount;
		return GetTotalObjCount();
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Decreases the counter for the total amount of objectives in the world.
	*/
	int DecTotalObjCount(int amount)
	{
		m_TotalObjCount -= amount;
		return GetTotalObjCount();
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Returns the an array of arrays, which should have two widgets each. These are the map marker widgets for each objective.
		Arrays should be in the following format:
		[
			[ImageWidget, TextWidget],
			[ImageWidget, TextWidget],
			[ImageWidget, TextWidget],
			[etc]
		]
	*/
	array<array<Widget>> GetObjMarkerArray()
	{
		return m_ObjMarkerArr;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Adds the widget array to a main array.
	*/
	void AddObjMarker(array<Widget> widgets)
	{
		m_ObjMarkerArr.Insert(widgets);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Adds an objective to the hostile objective array, as of 0.1.04 also increases the total objective count.
		/param obj should be the IEntity that has all of our components attached to it, such as LIB_ObjectiveComponent.
	*/
	void AddHostileObjArray(IEntity obj)
	{
		m_HostileObjArr.Insert(obj);
		m_TotalObjCount += 1;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Removes an objective from the hostile objective array, does not affect total objective count.
		/param obj should be the IEntity that has all of our components attached to it, such as LIB_ObjectiveComponent.
	*/
	void RemHostileObj(IEntity obj)
	{
		m_HostileObjArr.RemoveItem(obj);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Gets the array hostile objective entities.
	*/
	array<IEntity> GetHostileObjArray()
	{
		return m_HostileObjArr;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Gets a random objective from the array of hostile objectives.
	*/
	IEntity GetRandHostileObj()
	{
		if (m_HostileObjArr.IsEmpty()) {
			return null;
		};
		
		return m_HostileObjArr.GetRandomElement();
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Adds an objective to the friendly objective array, as of 0.1.04 also increases the total objective count.
		/param obj should be the IEntity that has all of our components attached to it, such as LIB_ObjectiveComponent.
	*/
	void AddFriendlyObjArray(IEntity obj)
	{
		m_FriendlyObjArr.Insert(obj);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Removes an objective from the friendly objective array, does not affect total objective count.
		/param obj should be the IEntity that has all of our components attached to it, such as LIB_ObjectiveComponent.
	*/
	void RemFriendlyObj(IEntity obj)
	{
		m_FriendlyObjArr.RemoveItem(obj);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Gets the array of Friendly objective entities.
	*/
	array<IEntity> GetFriendlyObjArray()
	{
		return m_FriendlyObjArr;
	}
	//------------------------------------------------------------------------------------------------
	/*!
		Gets an array of what objectives are currently active/spawned.
	*/
	array<IEntity> GetObjActiveArray()
	{
		return m_ObjActiveArr;
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Adds an objective to the list of currently active objectives.
	*/
	void AddObjActive(IEntity ent)
	{
		m_ObjActiveArr.Insert(ent);
		
		// Call UI element onto all players screens if not already, and add objective name, cancel remove from screen delay if active (Probably should make another function)
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Removes an objective from the list of currently active objectives.
	*/
	void RemObjActive(IEntity ent)
	{
		m_ObjActiveArr.RemoveItem(ent);
		
		// Remove objective name from ui element, and if element is empty remove from player screen after a delay.
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
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Should handle popping up the UI on players screens to announce
	*/
	void TellObjCaptured(IEntity obj)
	{
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Should handle popping up the UI on players screens to announce
	*/
	void TellObjLost(IEntity obj)
	{
	}
};
