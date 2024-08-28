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
	void buildStructureServer(ResourceName resourcename, vector mat[4])
	{
		Rpc(RpcAsk_BuildStructureServer, resourcename, mat);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_BuildStructureServer(ResourceName resourcename, vector mat[4])
	{
		LIB_BuildStructureWIP builder = new LIB_BuildStructureWIP;
		builder.build(resourcename, mat);
	}

	//------------------------------------------------------------------------------------------------
	/*void addBuildRequestServer(ResourceName resourcename, EntityID groupID, RplId StructRplId, vector mat[4], int playerId, int buildTime,)
	{
		Rpc(RpcAsk_AddBuildRequestServer, resourcename, groupID, StructRplId, mat, playerId, buildTime);
	}*/

	//------------------------------------------------------------------------------------------------
	/*[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_AddBuildRequestServer(ResourceName resourcename,EntityID groupID, RplId StructRplId, vector mat[4], int playerId, int buildTime,)
	{
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(StructRplId));
		IEntity factory = rplComp.GetEntity();
		SCR_CTI_BuildQueueComponent bqcomp = SCR_CTI_BuildQueueComponent.Cast(factory.FindComponent(SCR_CTI_BuildQueueComponent));
		bqcomp.addRequest(resourcename, groupID, factRplId, mat, playerId, buildTime, purchaseInfo);
	}*/

	//------------------------------------------------------------------------------------------------
	/*void changeFundsServer(int playerId, int value)
	{
		Rpc(RpcAsk_changeFundsServer, playerId, value);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_changeFundsServer(int playerId, int value)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_ClientData clientData = gameMode.getClientData(playerId);
		if (clientData) clientData.changeFunds(value);
		gameMode.bumpMeServer();
	}
	
	//------------------------------------------------------------------------------------------------
	void changeCommanderFundsServer(FactionKey factionkey, int value)
	{
		Rpc(RpcAsk_changeCommanderFundsServer, factionkey, value);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_changeCommanderFundsServer(FactionKey factionkey, int value)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		gameMode.changeCommanderFunds(factionkey, value);
	}
	
	//------------------------------------------------------------------------------------------------
	void setPriorityServer(FactionKey factionkey, string townName)
	{
		Rpc(RpcAsk_setPriorityServer, factionkey, townName);
	}
	
	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_setPriorityServer(FactionKey factionkey, string townName)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		gameMode.setPriority(factionkey, townName);
	}

	//------------------------------------------------------------------------------------------------
	void savePlayerLoadout(int playerId, RplId rplid)
	{
		Rpc(RpcAsk_savePlayerLoadoutServer, playerId, rplid);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_savePlayerLoadoutServer(int playerId, RplId rplid)
	{
		SCR_ArsenalManagerComponent arsenalManager;
		SCR_ArsenalManagerComponent.GetArsenalManager(arsenalManager);
		
		RplComponent rplComp = RplComponent.Cast(Replication.FindItem(rplid));
		IEntity ent = rplComp.GetEntity();

		arsenalManager.SetPlayerArsenalLoadout(playerId, GameEntity.Cast(ent), null, SCR_EArsenalSupplyCostType.DEFAULT);
	}
	
	//------------------------------------------------------------------------------------------------
	void repairVehicleServer(int playerId, RplId rplid, int delay)
	{
		Rpc(RpcAsk_RepairVehicleServer, playerId, rplid, delay);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_RepairVehicleServer(int playerId, RplId rplid, int delay)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_ServiceComponent sc = SCR_CTI_ServiceComponent.Cast(gameMode.FindComponent(SCR_CTI_ServiceComponent));
		GetGame().GetCallqueue().CallLater(sc.delayedRepair, delay * 1000, false, playerId, rplid);
	}

	//------------------------------------------------------------------------------------------------
	void rearmVehicleServer(int playerId, RplId rplid, int delay)
	{
		Rpc(RpcAsk_RearmVehicleServer, playerId, rplid, delay);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_RearmVehicleServer(int playerId, RplId rplid, int delay)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_ServiceComponent sc = SCR_CTI_ServiceComponent.Cast(gameMode.FindComponent(SCR_CTI_ServiceComponent));
		GetGame().GetCallqueue().CallLater(sc.delayedRearm, delay * 1000, false, playerId, rplid);
	}

	//------------------------------------------------------------------------------------------------
	void refuelVehicleServer(int playerId, RplId rplid, int delay)
	{
		Rpc(RpcAsk_RefuelVehicleServer, playerId, rplid, delay);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_RefuelVehicleServer(int playerId, RplId rplid, int delay)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_ServiceComponent sc = SCR_CTI_ServiceComponent.Cast(gameMode.FindComponent(SCR_CTI_ServiceComponent));
		GetGame().GetCallqueue().CallLater(sc.delayedRefuel, delay * 1000, false, playerId, rplid);
	}

	//------------------------------------------------------------------------------------------------
	void healVehicleCrewServer(int playerId, RplId rplid, int delay)
	{
		Rpc(RpcAsk_HealVehicleCrewServer, playerId, rplid, delay);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_HealVehicleCrewServer(int playerId, RplId rplid, int delay)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_ServiceComponent sc = SCR_CTI_ServiceComponent.Cast(gameMode.FindComponent(SCR_CTI_ServiceComponent));
		GetGame().GetCallqueue().CallLater(sc.delayedHeal, delay * 1000, false, playerId, rplid);
	}

	//------------------------------------------------------------------------------------------------
	void addWorkerServer(FactionKey factionkey)
	{
		Rpc(RpcAsk_addWorkerServer, factionkey);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_addWorkerServer(FactionKey factionkey)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_UpdateWorkersComponent uwc = SCR_CTI_UpdateWorkersComponent.Cast(gameMode.FindComponent(SCR_CTI_UpdateWorkersComponent));
		if (uwc) uwc.addWorker(factionkey);
	}
	
	//------------------------------------------------------------------------------------------------
	void disbandWorkerServer(FactionKey factionkey, RplId rplid)
	{
		Rpc(RpcAsk_disbandWorkerServer, factionkey, rplid);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_disbandWorkerServer(FactionKey factionkey, RplId rplid)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_UpdateWorkersComponent uwc = SCR_CTI_UpdateWorkersComponent.Cast(gameMode.FindComponent(SCR_CTI_UpdateWorkersComponent));
		if (uwc) uwc.disbandWorker(factionkey, rplid);
	}
	
	//------------------------------------------------------------------------------------------------
	void deleteObjectServer(FactionKey factionkey, RplId rplid)
	{
		Rpc(RpcAsk_deleteObjectServer, factionkey, rplid);
	}

	//------------------------------------------------------------------------------------------------
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_deleteObjectServer(FactionKey factionkey, RplId rplid)
	{
		SCR_CTI_GameMode gameMode = SCR_CTI_GameMode.GetInstance();
		SCR_CTI_BaseComponent baseComp = SCR_CTI_BaseComponent.Cast(gameMode.FindComponent(SCR_CTI_BaseComponent));
		if (baseComp) baseComp.deleteObject(factionkey, rplid);
	}*/

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