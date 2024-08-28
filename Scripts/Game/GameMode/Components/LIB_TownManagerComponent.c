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
		
		obj = Entity with all objective related components on it
		hide = Will hide the ui instead of creating a new ui, or setting an existing ui visible.
	*/
	void TellObjCaptured(IEntity obj, bool hide = false)
	{
		TellObjUIChange(obj, "ObjCapturedFrame", "{7BAC737E810740B2}UI/layouts/Objectives/ObjCaptured.layout", true);
	}
	
	//------------------------------------------------------------------------------------------------
	/*!
		Should handle popping up the UI on players screens to announce
	*/
	void TellObjLost(IEntity obj, bool hide = false)
	{
		TellObjUIChange(obj, "ObjCapturedFrame", "{7BAC737E810740B2}UI/layouts/Objectives/ObjCaptured.layout", false);
	}
	
	void TellObjUIChange(IEntity obj, string widgetName, ResourceName widgetPath, bool captured, bool hide = false)
	{
		LIB_ObjectiveComponent objComp = LIB_ObjectiveComponent.Cast(obj.FindComponent(LIB_ObjectiveComponent));
		
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		
		Widget WidgetLayer = workspace.FindWidget(widgetName);
		
				
		if (!WidgetLayer)
		{
			WidgetLayer = workspace.CreateWidgets(widgetPath);
		}
		
		ImageWidget uiImage = ImageWidget.Cast(WidgetLayer.FindAnyWidget("Image0"));
		if (uiImage && !hide)
		{
			uiImage.LoadImageFromSet(0, "{B003950B54B7AA46}UI/Textures/Icons/lib_markers.imageset", objComp.GetObjTypeToString(objComp.GetObjType()));
			uiImage.SetColor(Color.Blue);
		}
		
		uiImage.SetVisible(!hide);

		string capturedString = "captured";
		if (!captured)
			capturedString = "false";
		
		TextWidget uiText = TextWidget.Cast(WidgetLayer.FindAnyWidget("RichText0"));
		if (uiText && !hide)
		{
			uiText.SetColor(Color.Black);
			uiText.SetText(string.Format("Objective %1 has been %2!", objComp.GetObjName(), capturedString));
		}
		uiText.SetVisible(!hide);
		
		if (!hide)
			GetGame().GetCallqueue().CallLater(TellObjUIChange, 15000, false, obj, widgetName, widgetPath, captured, true);
	}
};
