class LIB_BuildingMenuManagerClass : ScriptComponentClass
{
}

class LIB_BuildingMenuManager : ScriptComponent
{
	protected TRA_GlobalConfig m_config = TRA_GlobalConfig.GetConfig();
	protected InputManager m_InpMngr;
	protected PlayerController m_PlayerController;
	
	protected bool m_BuildMenuOpen = false;
	
	protected Widget m_BuildMenuWidget;
	
	void OpenBuildMenuUI(string widgetName, ResourceName widgetPath, bool remove = false)
	{
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		
		// If widget not already assigned, look for it
		if (!m_BuildMenuWidget)
			m_BuildMenuWidget = workspace.FindWidget(widgetName);
		
		// if still not assigned, create it				
		if (!m_BuildMenuWidget)
			m_BuildMenuWidget = workspace.CreateWidgets(widgetPath);
		
		m_BuildMenuOpen = true;
		
		TextListboxWidget ItemCategory = TextListboxWidget.Cast(m_BuildMenuWidget.FindAnyWidget("category_items"));
		TextListboxWidget PriceCategory = TextListboxWidget.Cast(m_BuildMenuWidget.FindAnyWidget("category_prices"));
		
		int lastRow = -1;
		int curRow = lastRow;
		if (lastRow > -1)
			curRow = lastRow + 1;
		
		foreach(LIB_BuildItemData item : m_config.m_BFobBuildList)
		{
			lastRow = ItemCategory.AddItem(item.GetItemName(), item, 0, curRow);
			curRow++;
		}
		
		ButtonWidget btnBuild = ButtonWidget.Cast(m_BuildMenuWidget.FindAnyWidget("button_build"));
		SCR_ModularButtonComponent modBtnComp = SCR_ModularButtonComponent.FindComponent(btnBuild);
		modBtnComp.m_OnClicked.Insert(testFunc);
		
		if (remove)
			SCR_WidgetHelper.RemoveAllChildren(m_BuildMenuWidget);
	}
	
	void testFunc()
	{
		Print("Test Func");
		// using m_BuildMenuWidget find the TextListboxWidget, get the selected row, grab managed userdata, test
		TextListboxWidget ItemCategory = TextListboxWidget.Cast(m_BuildMenuWidget.FindAnyWidget("category_items"));
		Print(ItemCategory);
		
		int SelectedRow = ItemCategory.GetSelectedRow();
		Print(SelectedRow);
		
		string SelectedText;
		ItemCategory.GetItemText(SelectedRow, 0, SelectedText);
		Print(SelectedText);
		
		Managed SelectedData = ItemCategory.GetItemData(SelectedRow);
		Print(SelectedData);
	}
	
	void BlockControlsForUI()
	{
		
	}
	
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}
	
	override void EOnInit(IEntity owner)
	{
		m_InpMngr = GetGame().GetInputManager();
		m_PlayerController = PlayerController.Cast(owner);
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (m_BuildMenuOpen)
		{
			m_InpMngr.ActivateContext("LibUi", 1);
		}
	}
}