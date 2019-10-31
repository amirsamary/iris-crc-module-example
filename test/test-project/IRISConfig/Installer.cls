Include %occInclude

Class IRISConfig.Installer Extends IRISConfig.InstallerBase
{
		
	XData Install [ XMLNamespace = INSTALLER ]
	{
	<Manifest>
		
		<Namespace Name="${Namespace}" Create="no">
			        
			<IfDef Var="SourceDir">
	            <Log Text="SourceDir defined - offline install from ${SourceDir}" Level="0"/>
	            <Import File="${SourceDir}" Recurse="true"/>
	        </IfDef>

		</Namespace>
		
	</Manifest>
	}
}