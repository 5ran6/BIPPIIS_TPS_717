package com.greenbit.ansinistitl;

public class GBAN2011JavaWrapperDefinesGmtStruct
{
	public int Year;
	public int Month;
	public int Day;
	public int hour;
	public int minute;
	public int second;

	public void Build(int year, int month, int day, int h, int m, int s)
	{
		Year = year;
		Month = month;
		Day = day;
		hour = h;
		minute = m;
		second = s;
	}
}
