

print("Loading script");

scriptRet = cppClassTable_AnimationState:Create(
		{ 
		RootAnimNode = cppClassTable_AnimationClipNode:Create( 
				{
				ClipName = "helloWOrld",
				
				ChildAnimNodes = { }
				} 	
			)
		} 
	);
	
	
print("Loading script complete");