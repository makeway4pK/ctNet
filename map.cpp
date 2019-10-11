#include<iostream>
#include<fstream>
#include<string.h>
#define MAX 7	
using namespace std;


typedef struct ticket
{
	string name,from,to,flight;
	int n,id, day,month,year;
}ticket;

class AEROPLANE
{
	
};

class flight
{
	int row[3],countp,countf,np;
	char no;
	int tseat[MAX];
	char tno[MAX];
	int cost;
	ticket t;	
	public:
	flight()
	{
		row[0]=1,row[1]=0,row[2]=0,countp=0;
		no = '@';
		t.id=0;
	}
	
	void start()
	{
		while(1)
		{
			cout<<"\nEnter today's DATE : \nDAY : ";
			cin>>t.day;
			cout<<"\nMonth : ";
			cin>>t.month;
			cout<<"\nYear : ";
			cin>>t.year;
			if((t.day>=31 && (t.month==4||t.month==6||t.month==9||t.month==11))||(t.day>=32 && (t.month==1||t.month==3||t.month==5||t.month==7||t.month==8||t.month==10))||(t.day>=30 && t.month == 2 && t.year%4==0)||(t.day>=29 && t.month == 2 && t.year%4!=0))
			{
				cout<<"\nINVALID DATE...Please enter correct date.\n";
				continue;
			}
			else
				break;
		}
		if(t.day == 31 && t.month == 12 )
		{
			t.day=1;
			t.month=1;
			t.year++;
		}	 
		else if((t.day == 30 && (t.month==4||t.month==6||t.month==9||t.month==11))||(t.day == 31 && (t.month==1||t.month==3||t.month==5||t.month==7||t.month==8||t.month==10)))
		{
			t.day=1;
			t.month++;
		}
		
		else if((t.day==29 && t.month == 2 && t.year%4==0)||(t.day==28 && t.month == 2))
		{
			t.day=1;
			t.month++;
		}
		
		else
		{
			t.day++;
		}		
	}

	/*int cost_cal(int time,int p)
	{
		return p*((time*100)/4);
	}*/
	
	void book()
	{
		start();
		cout<<"\nEnter the number of passengers : ";
		cin>>np;
		try
		{		
			if(np>7)
				throw np;
			
			countp=countp+np;
			if(countp<=50)
			{
				if(np==1)
					cout<<"\nEnter Name of Passenger : ";
				else
				cout<<"\nEnter name of the HEAD of your group : ";
				cin>>t.name;					
				t.id++;
				t.n=np;
				
				cout<<"\nEnter SOURCE : ";
				cin>>t.from;
				cout<<"\nEnter DESTINATION : ";
				cin>>t.to;
				
				for(int i=0;i<t.n;i++)
				{
					seat();
					tseat[i]=row[1]*10+row[0];
					tno[i]=no;
				}
						
				t.flight="AIR_INDIA";
				ofstream file;
				file.open("BOOKINGS.DAT",ios::app);
				file.write((char*)&t,sizeof(t));
				ISSUE_ticket();
			}
			else
				cout<<"\nFlight is FULL !!! Please try again tommorow.\n";	
		}
		catch(int a)
		{
			cout<<"\nEXCEPTION CAUGHT!!!";
			cout<<"\nYOU CANNOT BOOK MORE THAN 7 TICKETS\n";
		}		
	}
	
	
	void ISSUE_ticket()
	{
		cout<<"\n\t\t\t\t|**************************************************************************************************************|\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|\t"<<t.flight<<"\t\t\t\t\t\t\t\t\t\t"<<"ID : "<<t.id<<"\t\t   |"<<"\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|**************************************************************************************************************|\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|\tPassenger Name : "<<t.name<<"\t\t\t\t\t\tNumber of seats : "<<t.n;
		if(np>1)
			cout<<"\n\t\t\t\t|                                                                                                                  |\n\t\t\t\t|\tFrom : "<<t.from<<"\t\t\t\t\t\t\t\t\tSeat No : "<<tseat[0]<<tno[0]<<" to "<<tseat[t.n-1]<<tno[t.n-1]<<"\t   |";
		else
			cout<<"\n\t\t\t\t|                                                                                                                  |\n\t\t\t\t|\tFrom : "<<t.from<<"\t\t\t\t\t\t\t\t\tSeat No : "<<tseat[0]<<tno[0]<<"\t\t   |";
		cout<<"\n\t\t\t\t|                                                                                                                  |\n\t\t\t\t|\tTo : "<<t.to<<"\t\t\t\t\t\t\t\t\tDate : "<<t.day<<"/"<<t.month<<"/"<<t.year<<"\t\t   |\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|**************************************************************************************************************|\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|\t\t\t\t\t\tHAPPY JOURNEY\t\t\t\t\t\t\t   |\n\t\t\t\t|                                                                                                                  |";
		cout<<"\n\t\t\t\t|**************************************************************************************************************|\n";
	}	
	void seat()
	{
		if(row[1]<1)
		{
			int t=(int) no;
			if(t<69)
			{
				t++;
				no = (char) t;
			}
			else if(t==69)
			{
				no = 'A';
				if(row[0]==9)
				{
					if(row[1]<1)
					{		
						row[1]++;
					}	
				}
				else
					row[0]++;
			}
		}
	}
};	






int main()
{
	flight f;
	f.book();
	return 0;
}

