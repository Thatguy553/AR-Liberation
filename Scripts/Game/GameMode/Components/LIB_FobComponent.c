class LIB_FobComponentClass : ScriptComponentClass
{
}

class LIB_FobComponent : ScriptComponent
{
	//[Attribute(category: "", defvalue: "", desc: "")]
	[Attribute(defvalue: "FOB Default", desc: "FOB Name")]
	protected string m_fobName;
	
	[Attribute(defvalue: "200", desc: "Build Radius around the FOB building")]
	protected float m_fobRadius;
	
	[Attribute(defvalue: "0", desc: "Starting FOB Supply")]
	protected int m_fobSupplies;
	
	[Attribute(defvalue: "0", desc: "Starting FOB Ammo")]
	protected int m_fobAmmunition;
	
	[Attribute(defvalue: "0", desc: "Starting FOB Fuel")]
	protected int m_fobFuel;
	
	// Structures built by players at this FOB. - Should not despawn
	protected array<RplId> m_fobProps;
	
	// Vehicles built by this fob, or parked within radius of this FOB. - Should not despawn
	protected array<RplId> m_fobVehs;
	
	// The FOB building entity, this component should be attached to it. 
	protected IEntity m_fobEnt;
	
	// The FOB's related faction, may be used in the future for scenarios where FOB's are captured rather than destroying them
	[Attribute(defvalue: "US", desc: "FOB Faction")]
	protected FactionKey m_fobFaction;
	
	// Should store the FOB's world position, and its rotation.
	protected vector m_fobPos;
	
	void SetFobName(string name)
	{
		m_fobName = name;
	}
	
	string GetFobName()
	{
		return m_fobName;
	}
	
	void SetFobRadius(float radius)
	{
		m_fobRadius = radius;
	}
	
	float GetFobRadius()
	{
		return m_fobRadius;
	}
	
	void GetFobResources(out int supplies, out int fuel, out int ammo)
	{
		supplies = m_fobSupplies;
		fuel = m_fobFuel;
		ammo = m_fobAmmunition;
	}
	
	// Increases fob resources by supplied numbers, 0 = no change, negative number decreases, positive increases
	void SetFobResources(int supplies = 0, int ammo = 0, int fuel = 0)
	{
		m_fobSupplies += supplies;
		m_fobAmmunition += ammo;
		m_fobFuel += fuel;
	}
	
	void AddFobProps(RplId rplid)
	{
		m_fobProps.Insert(rplid);
	}
	
	void AddFobProps(array<RplId> rplids)
	{
		m_fobProps.InsertAll(rplids);
	}
	
	array<RplId> GetFobProps()
	{
		return m_fobProps;
	}
	
	void RemoveFobProp(RplId rplid)
	{
		m_fobProps.RemoveItem(rplid);
	}
	
	// Removes the rplid from this fob, also deletes the IEntity belonging to that ID.
	// returns -1 if prop not found in this FOB's prop array
	int DeleteFobProp(RplId rplid)
	{
		if (m_fobProps.Find(rplid) <= -1)
			return -1;
		
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(rplid));
		
		SCR_EntityHelper.DeleteEntityAndChildren(rplComp.GetEntity());
		
		m_fobProps.RemoveItem(rplid);
		
		return 1;
	}
}