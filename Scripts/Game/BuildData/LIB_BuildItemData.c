[BaseContainerProps()]
class LIB_BuildItemData
{

	[Attribute(desc: "Item Category")]
	protected string m_sItemCategory;
	
	// Just for visual in menu
	[Attribute(desc: "Item Name")]
	protected string m_sItemName;
	
	[Attribute(desc: "Resrouce Name of item to be spawned.")]
	protected ResourceName m_sItemRes;
	
	[Attribute(desc: "Supply Cost of Item")]
	protected int m_iItemSupplyCost;
	
	[Attribute(desc: "Fuel Cost of Item")]
	protected int m_iItemFuelCost;
	
	[Attribute(desc: "Ammo Cost of Item")]
	protected int m_iItemAmmoCost;
	
	string GetItemName()
	{
		return m_sItemName;
	}
	
	ResourceName GetItemRes()
	{
		return m_sItemRes;
	}
	
	string GetItemCategoy()
	{
		return m_sItemCategory;
	}
	
	int GetItemSupplyCost()
	{
		return m_iItemSupplyCost;
	}
	
	int GetItemFuelCost()
	{
		return m_iItemFuelCost;
	}
	
	int GetItemAmmoCost()
	{
		return m_iItemAmmoCost;
	}
	
	void SetItemName(string name)
	{
		m_sItemName = name;
	}
	
	void SetItemRes(ResourceName res)
	{
		m_sItemRes = res;
	}
	
	void SetItemSupplyCost(int cost)
	{
		m_iItemSupplyCost = cost;
	}
	
	void SetItemFuelCost(int cost)
	{
		m_iItemFuelCost = cost;
	}
	
	void SetItemCategory(string category)
	{
		m_sItemCategory = category;
	}
	
	void SetItemAmmoCost(int cost)
	{
		m_iItemAmmoCost = cost;
	}
}