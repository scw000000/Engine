

print("Loading script");

--[[
scriptRet = cppClassTable_AnimationState:Create(
		{ 
		RootAnimNode = cppClassTable_AnimationClipNode:Create( 
				{
				ClipName = "art\\run_left_inPlace.fbx",
				
				ChildAnimNodes = { }
				} 	
			)
		} 
	);
    .RootAnimNode:SetLerpFactor( 1.0 );
]]--
        
scriptRet = cppClassTable_AnimationState:Create(
		{
        RootAnimNode = cppClassTable_AnimationLerpNode:Create( 
				{	
                LeftChild = cppClassTable_AnimationClipNode:Create( 
                    {
                    ClipName = "art\\run_left_inPlace.fbx",
				
                    ChildAnimNodes = { }
                    }
                    ),
                    
                RightChild = cppClassTable_AnimationClipNode:Create( 
                    {
                    ClipName = "art\\run_right_inPlace_Mirrored.fbx",
				
                    ChildAnimNodes = { }
                    } 	
                    ),
				ChildAnimNodes = { }
				} 	
			)
		} 
	);
   
--RootAnimNodeData:SetLerpFactor( 1.0 );  
scriptRet.RootAnimNode:SetLerpFactor( 0.5 );
--scriptRet.RootAnimNodeData:SetLerpFactor( 1.0 );
	
print("Loading script complete");