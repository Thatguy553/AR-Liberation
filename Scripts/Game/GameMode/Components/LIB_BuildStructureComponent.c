// Credits to HTomJMW for this script
[EntityEditorProps(category: "GameScripted/Building", description: "Should be attached to PlayerController.")]
class LIB_BuildStructureComponentClass: ScriptComponentClass
{
};

class LIB_BuildStructureComponent : ScriptComponent
{
	protected PlayerController m_PlayerController;
	protected RplComponent m_RplComponent;
	protected InputManager InpMngr;

	//protected FactionKey m_factionkey;
	protected ResourceName m_resName;

	protected SCR_BasePreviewEntity m_structure = null;
	
	protected vector finalMat[4];
	
	protected bool m_startPlacing = false;
	
	protected float m_dist;
	protected int m_placement;

	protected ResourceName m_material = "{58F07022C12D0CF5}Assets/Editor/PlacingPreview/Preview.emat";
	protected ResourceName m_materialRed = "{F34CA01A59FDBED4}Assets/Editor/PlacingPreview/PreviewRed.emat";
	
	protected bool m_isFOB = false;
	
	protected bool m_blocked = false;
	protected float m_radius;
	protected float m_maxDifference;
	
	protected ref TraceOBB m_paramOBB = new TraceOBB();
	
	protected vector m_boundmins;
	protected vector m_boundmaxs;
	
	protected float m_fMatX = 0;
	protected float m_fMatY = 0;
	protected float m_fMatZ = 0;
	protected float m_fMatR = 0;

	//------------------------------------------------------------------------------------------------
	bool getStartPlacing()
	{
		return m_startPlacing;
	}

	//------------------------------------------------------------------------------------------------
	void StructMoveDown()
	{
		m_fMatY -= 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructMoveUp()
	{
		m_fMatY += 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructMoveLeft()
	{
		m_fMatZ += 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructMoveRight()
	{
		m_fMatZ -= 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructMoveForward()
	{
		m_fMatX -= 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructMoveBackward()
	{
		m_fMatX += 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructRotateLeft()
	{
		m_fMatR += 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void StructRotateRight()
	{
		m_fMatR -= 1;
	}
	
	//------------------------------------------------------------------------------------------------
	void createStructurePreview(ResourceName resName, float distance, int placement, bool startPlacing, bool isFOB = false)
	{
		// Start listening for input
		InpMngr.AddActionListener("LibBuildingQ", EActionTrigger.PRESSED, StructMoveDown);
		InpMngr.AddActionListener("LibBuildingE", EActionTrigger.PRESSED, StructMoveUp);
		
		InpMngr.AddActionListener("LibBuildingUp", EActionTrigger.PRESSED, StructMoveForward);
		InpMngr.AddActionListener("LibBuildingDown", EActionTrigger.PRESSED, StructMoveBackward);
		
		InpMngr.AddActionListener("LibBuildingLeft", EActionTrigger.PRESSED, StructMoveLeft);
		InpMngr.AddActionListener("LibBuildingRight", EActionTrigger.PRESSED, StructMoveRight);
		
		InpMngr.AddActionListener("LibBuildingLeftBracket", EActionTrigger.PRESSED, StructRotateLeft);
		InpMngr.AddActionListener("LibBuildingRightBracket", EActionTrigger.PRESSED, StructRotateRight);
		
		InpMngr.AddActionListener("LibBuildingMouse0", EActionTrigger.PRESSED, performBuilding);
		InpMngr.AddActionListener("LibBuildingMouse1", EActionTrigger.PRESSED, cancelBuilding);

		// Create UI
		OpenBuildControlUI("BuildControls", "{0DF839AA29EE4320}UI/layouts/BuildingUI/BuildingHUD.layout");
		
		m_isFOB = isFOB;
		m_resName = resName;
		m_dist = distance;
		m_placement = placement;
		
		vector mat[4];
		vector dir;
		
		IEntity m_player = m_PlayerController.GetControlledEntity();
		m_player.GetTransform(mat);
		dir = m_player.GetWorldTransformAxis(2);

		vector angles = Math3D.MatrixToAngles(mat);
		angles[0] = angles[0] + m_placement;
		Math3D.AnglesToMatrix(angles, mat);

		mat[3] = mat[3] + (dir * m_dist);
		mat[3][1] = GetGame().GetWorld().GetSurfaceY(mat[3][0], mat[3][2]);
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = mat;
		
		Resource resource = Resource.Load(resName);

		m_structure = SCR_PrefabPreviewEntity.SpawnPreviewFromPrefab(resource, "SCR_PrefabPreviewEntity", GetGame().GetWorld(), params, m_material);

		if (m_structure)
		{
			m_structure.Update();
			PrintFormat("LIB :: Created Structure preview: %1", m_structure);

			m_structure.GetPreviewBounds(m_boundmins, m_boundmaxs);
			m_radius = (vector.DistanceXZ(m_boundmins, m_boundmaxs)) * 0.5;
			m_maxDifference = m_radius * 0.15;
			
			Math3D.MatrixIdentity3(m_paramOBB.Mat);
			m_paramOBB.Flags = TraceFlags.ENTS;
			m_paramOBB.Exclude = m_structure;
			m_paramOBB.LayerMask = EPhysicsLayerPresets.Building;
			m_paramOBB.Mins = m_boundmins;
			m_paramOBB.Maxs = m_boundmaxs;

			m_startPlacing = startPlacing;
			
			GetGame().GetCallqueue().CallLater(updatePreiew, 25, true);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected void updatePreiew()
	{
		vector mat[4];
		vector dir;
		
		IEntity m_player = m_PlayerController.GetControlledEntity();
		if (!m_player)
		{
			cancelBuilding();
			return;
		}
		m_player.GetTransform(mat);
		
		dir = m_player.GetWorldTransformAxis(2);
		
		vector angles = Math3D.MatrixToAngles(mat);
		angles[0] = (angles[0] + m_placement) + m_fMatR;
		Math3D.AnglesToMatrix(angles, mat);
		
		mat[3] = mat[3] + (dir * m_dist);
		
		mat[3][1] = mat[3][1] + m_fMatY;
		mat[3][0] = mat[3][0] + m_fMatZ;
		mat[3][2] = mat[3][2] + m_fMatX;
		
		SetHierarchyTransform(m_structure, mat);
		
		m_paramOBB.Mat[0] = mat[0];
		m_paramOBB.Mat[1] = mat[1];
		m_paramOBB.Mat[2] = mat[2];
		m_paramOBB.Start = mat[3] + "0 0.05 0";
		
		GetGame().GetWorld().TracePosition(m_paramOBB, null);
		
		if (!isFlat(mat[3], m_radius, m_maxDifference) || mat[3][1] < 0 || m_paramOBB.TraceEnt)
		{
			SCR_Global.SetMaterial(m_structure, m_materialRed);
			m_blocked = true;
		} else {
			SCR_Global.SetMaterial(m_structure, m_material);
			m_blocked = false;
		}
		
		m_structure.Update();

		finalMat = mat;
	}
	
	//------------------------------------------------------------------------------------------------
	void performBuilding()
	{
		if (m_blocked) return;

		auto menuManager = GetGame().GetMenuManager();
		menuManager.CloseAllMenus();

		m_startPlacing = false;

		GetGame().GetCallqueue().Remove(updatePreiew);
		removeBuildingInputs();
		OpenBuildControlUI("BuildControls", "{0DF839AA29EE4320}UI/layouts/BuildingUI/BuildingHUD.layout", true);
		
		SCR_EntityHelper.DeleteEntityAndChildren(m_structure);
		
		LIB_NetworkComponent netComp = LIB_NetworkComponent.Cast(m_PlayerController.FindComponent(LIB_NetworkComponent));
		
		// Should make buildStructureServer return a bool depending on success or not.
		netComp.buildStructureServer(m_resName, finalMat, m_isFOB);
	}

	//------------------------------------------------------------------------------------------------
	void cancelBuilding()
	{
		auto menuManager = GetGame().GetMenuManager();
		menuManager.CloseAllMenus();
		
		m_startPlacing = false;

		GetGame().GetCallqueue().Remove(updatePreiew);
		removeBuildingInputs();
		OpenBuildControlUI("BuildControls", "{0DF839AA29EE4320}UI/layouts/BuildingUI/BuildingHUD.layout", true);

		if (m_structure) SCR_EntityHelper.DeleteEntityAndChildren(m_structure);
	}

	//------------------------------------------------------------------------------------------------
	void removeBuildingInputs()
	{
		InpMngr.RemoveActionListener("LibBuildingQ", EActionTrigger.PRESSED, StructMoveDown);
		InpMngr.RemoveActionListener("LibBuildingE", EActionTrigger.DOWN, StructMoveUp);
		InpMngr.RemoveActionListener("LibBuildingUp", EActionTrigger.PRESSED, StructMoveForward);
		InpMngr.RemoveActionListener("LibBuildingDown", EActionTrigger.PRESSED, StructMoveBackward);
		InpMngr.RemoveActionListener("LibBuildingLeft", EActionTrigger.PRESSED, StructMoveLeft);
		InpMngr.RemoveActionListener("LibBuildingRight", EActionTrigger.PRESSED, StructMoveRight);
		InpMngr.RemoveActionListener("LibBuildingMouse0", EActionTrigger.PRESSED, performBuilding);
		InpMngr.RemoveActionListener("LibBuildingMouse1", EActionTrigger.PRESSED, cancelBuilding);
	}
	
	//------------------------------------------------------------------------------------------------
	protected bool IsProxy()
	{
		return (m_RplComponent && m_RplComponent.IsProxy());
	}

	//------------------------------------------------------------------------------------------------
	override void EOnInit(IEntity owner)
	{
		InpMngr = GetGame().GetInputManager();
		m_PlayerController = PlayerController.Cast(owner);
		m_RplComponent = RplComponent.Cast(owner.FindComponent(RplComponent));
	}

	//------------------------------------------------------------------------------------------------
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT | EntityEvent.FRAME);
	}

	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice) //!EntityEvent.FRAME
	{
		if (m_startPlacing)
			InpMngr.ActivateContext("LibCharacterBuilding", 1);
	}
	
	//------------------------------------------------------------------------------------------------
	void LIB_BuildStructureComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
	}

	//------------------------------------------------------------------------------------------------
	void ~LIB_BuildStructureComponent()
	{
	}
	
	//------------------------------------------------------------------------------------------------
	bool isFlat(vector center, float radius, float maxDifference = 0.25)
	{
		float height = center[1];

		// step 1
		float checkPoint = center[0] + radius;
		float checkPointHeight = GetGame().GetWorld().GetSurfaceY(checkPoint, center[2]);
		
		if (checkPointHeight > height + maxDifference || checkPointHeight < height - maxDifference) return false;

		// step 2
		checkPoint = center[0] - radius;
		checkPointHeight = GetGame().GetWorld().GetSurfaceY(checkPoint, center[2]);
		
		if (checkPointHeight > height + maxDifference || checkPointHeight < height - maxDifference) return false;

		// step 3
		checkPoint = center[2] + radius;
		checkPointHeight = GetGame().GetWorld().GetSurfaceY(center[0], checkPoint);
		
		if (checkPointHeight > height + maxDifference || checkPointHeight < height - maxDifference) return false;
		
		// step 4
		checkPoint = center[2] - radius;
		checkPointHeight = GetGame().GetWorld().GetSurfaceY(center[0], checkPoint);
		
		if (checkPointHeight > height + maxDifference || checkPointHeight < height - maxDifference) return false;

		return true;
	}
	
	void SetHierarchyTransform(notnull IEntity ent, vector newTransform[4])
	{
		vector oldTransform[4];
		ent.GetTransform(oldTransform);
		ent.SetTransform(newTransform);

		IEntity child = ent.GetChildren();
		while (child)
		{
			SetHierarchyChildTransform(child, oldTransform, newTransform, true);
			child = child.GetSibling();
		}
	}
	
	void SetHierarchyChildTransform(notnull IEntity ent, vector oldTransform[4], vector newTransform[4], bool recursive = true)
	{
		vector mat[4];
		ent.GetTransform(mat);

		vector diffMat[4];
		Math3D.MatrixInvMultiply4(oldTransform, mat, diffMat);
		Math3D.MatrixMultiply4(newTransform, diffMat, mat);

		ent.SetTransform(mat);

		IEntity child = ent.GetChildren();
		while (child)
		{
			SetHierarchyChildTransform(child, oldTransform, newTransform, recursive);
			child = child.GetSibling();
		}
	}
	
	// BuildControls
	// {0DF839AA29EE4320}UI/layouts/BuildingUI/BuildingHUD.layout
	// OpenBuildControlUI("BuildControls", "{0DF839AA29EE4320}UI/layouts/BuildingUI/BuildingHUD.layout")
	void OpenBuildControlUI(string widgetName, ResourceName widgetPath, bool remove = false)
	{
		WorkspaceWidget workspace = GetGame().GetWorkspace();
		
		Widget WidgetLayer = workspace.FindWidget(widgetName);
		
				
		if (!WidgetLayer)
		{
			WidgetLayer = workspace.CreateWidgets(widgetPath);
		}
		
		if (remove)
			SCR_WidgetHelper.RemoveAllChildren(WidgetLayer);
	}
};