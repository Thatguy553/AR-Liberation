// Credits to HTomJMW for this script
class LIB_NetworkComponentClass: ScriptComponentClass
{
};

class LIB_NetworkComponent : ScriptComponent
{
	protected SCR_PlayerController m_PlayerController;
	protected RplComponent m_RplComponent;

	//------------------------------------------------------------------------------------------------
	protected bool IsProxy()
	{
		return (m_RplComponent && m_RplComponent.IsProxy());
	}

	//------------------------------------------------------------------------------------------------
	void unflipNearestVehicleServer(RplId vehRplId)
	{
		Rpc(RpcAsk_UnflipNearestVehicle, vehRplId);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_UnflipNearestVehicle(RplId vehRplId)
	{
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(vehRplId));
		IEntity vehicle = rplComp.GetEntity();
		VehicleWheeledSimulation_SA_B simulation = VehicleWheeledSimulation_SA_B.Cast(vehicle.FindComponent(VehicleWheeledSimulation_SA_B));
		
		if (Math.AbsFloat(simulation.GetSpeedKmh()) < 5)
		{
			CarControllerComponent_B carController = CarControllerComponent_B.Cast(vehicle.FindComponent(CarControllerComponent_B));
			if (carController) carController.SetPersistentHandBrake(true);
			
			Physics physics = vehicle.GetPhysics();
			physics.SetVelocity("0 4 0");

			vector angles = vehicle.GetAngles();
			if (angles[0] > 70 || angles[0] < -70) angles[0] = 0;
			if (angles[2] > 70 || angles[2] < -70) angles[2] = 0;
			vehicle.SetAngles(angles);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	void buildStructureServer(ResourceName resourcename, vector mat[4], bool isFOB)
	{
		Rpc(RpcAsk_BuildStructureServer, resourcename, mat, isFOB);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_BuildStructureServer(ResourceName resourcename, vector mat[4], bool isFOB)
	{
		LIB_BuildStructureWIP builder = new LIB_BuildStructureWIP;
		builder.build(resourcename, mat, isFOB);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		m_PlayerController = SCR_PlayerController.Cast(PlayerController.Cast(owner));
		
		if (!m_PlayerController)
		{
			Print("LIB :: LIB_NetworkComponent must be attached to PlayerController!", LogLevel.ERROR);
			return;
		}
		
		m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);
	}

	//------------------------------------------------------------------------------------------------
	void LIB_NetworkComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
	}

	//------------------------------------------------------------------------------------------------
	void ~LIB_NetworkComponent()
	{
	}
};