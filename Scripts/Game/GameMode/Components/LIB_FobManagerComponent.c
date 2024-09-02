class LIB_FobManagerComponentClass : ScriptComponentClass
{

}

class LIB_FobManagerComponent : ScriptComponent
{

	protected ref array<IEntity> m_FobArr;
	
	[RplProp(onRplName: "OnStructureAdded", condition: RplCondition.Custom, customConditionName: "RpcConditionMethod")]
	ref array<RplId> FobRplIds = {};
	
	[RplProp(onRplName: "OnStructureAdded", condition: RplCondition.Custom, customConditionName: "RpcConditionMethod")]
	ref array<RplId> WIPFobRplIds = {};
	
	//------------------------------------------------------------------------------------------------
	void SetFobArr(array<IEntity> arr)
	{
		m_FobArr = arr;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddFobArr(IEntity ent)
	{
		m_FobArr.Insert(ent);
	}
	
	//------------------------------------------------------------------------------------------------
	array<IEntity> GetFobArr()
	{
		return m_FobArr;
	}
	
	//------------------------------------------------------------------------------------------------
	int GetFobCount()
	{
		return m_FobArr.Count();
	}
	
	//------------------------------------------------------------------------------------------------
	void AddFobRplId(RplId rplid)
	{
		FobRplIds.Insert(rplid);
		
		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	// Proxys
	void OnStructureAdded()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	bool RpcConditionMethod()
	{
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void AddWIPFobRplId(RplId rplid)
	{
		WIPFobRplIds.Insert(rplid);

		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	void RemoveWIPStructureRplId(FactionKey factionkey, RplId rplid)
	{
		WIPFobRplIds.RemoveItem(rplid);

		Replication.BumpMe();
	}
	
	//------------------------------------------------------------------------------------------------
	void CreateFob(IEntity ent)
	{
	
	}
}