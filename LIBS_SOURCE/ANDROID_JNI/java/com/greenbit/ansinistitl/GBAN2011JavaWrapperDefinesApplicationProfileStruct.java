package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesApplicationProfileStruct
{
	public String AppProfOrg;
	public String AppProfName;
	public String AppProfVer;

	public void Build(String appProfOrg, String appProfName, String appProfVer)
	{
		AppProfOrg = appProfOrg;
		AppProfName = appProfName;
		AppProfVer = appProfVer;
	}

	public GBAN2011JavaWrapperDefinesApplicationProfileStruct(String appProfOrg, String appProfName, String appProfVer)
	{
		this.Build(appProfOrg,appProfName,appProfVer);
	}
}
