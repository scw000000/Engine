

print("Loading script");

scriptRet = cppClassTable_AnimationState:Create(
		{ 
		RootAnimNode = cppClassTable_AnimationClipNode:Create( 
				{
				ClipName = "mixamo.com",
				
				ChildAnimNodes = { }
				} 	
			)
		} 
	);
	
	
print("Loading script complete");