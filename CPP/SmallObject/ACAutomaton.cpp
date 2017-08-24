#include <cstdio>
#include <cstring>
#include <cctype>
FILE *in[10],*out[10],*source;
int n=10;
const int max_len=1000;
const int sigma_len=129;
int ne[max_len][sigma_len];
int data[max_len];
int sz;
void doit(int p)
{
	int ch,node=0,p1,ch1;
	while(true)
	{
		if(data[node]!=-1)
		{
			p1=data[node];
			do ch1=fgetc(in[p1]); while(ch=='\r');
			data[node]=-1;
			ne[node][ch1+1]=sz;
			memset(ne[sz],0,sizeof(ne[sz]));
			data[sz]=p1;
			sz++;
		}
		do ch=fgetc(in[p]); while(ch=='\r');
		if(!ne[node][ch+1])
		{
			ne[node][ch+1]=sz;
			memset(ne[sz],0,sizeof(ne[sz]));
			data[sz]=p;
			sz++;
			break;
		}
		else
			node=ne[node][ch+1];
	}
}
void print_tabs(int n)
{
	while(n--) fprintf(source,"    ");
}
void print_switch(int tabs,int node)
{
	int i,t;
	print_tabs(tabs);
	fprintf(source,"do ch=getchar(); while(ch==\'\\r\');\n");
	print_tabs(tabs);
	fprintf(source,"switch(ch) {\n");
	tabs++;
	for(i=0;i<sigma_len;i++)
		if(ne[node][i] && data[ne[node][i]]!=-1)
		{
			print_tabs(tabs);
			fprintf(source,"case %d:    //data%d\n",i-1,data[ne[node][i]]);
			tabs++;
			print_tabs(tabs);
			fprintf(source,"printf(\"");
			while((t=fgetc(out[data[ne[node][i]]]))!=EOF)
				if(isprint(t)) fprintf(source,"%c",t);
				else if(t!='\r') fprintf(source,"\\%03o",t);
			fprintf(source,"\");\n");
			print_tabs(tabs);
			fprintf(source,"break;\n");
			tabs--;
		}
		else if(ne[node][i])
		{
			print_tabs(tabs);
			fprintf(source,"case %d:\n",i-1);
			tabs++;
			print_switch(tabs,ne[node][i]);
			print_tabs(tabs);
			fprintf(source,"break;\n");
			tabs--;
		}
	tabs--;
	print_tabs(tabs);
	fprintf(source,"}\n");
}
void print()
{
	int tabs=0;
	fprintf(source,"#include <stdio.h>\nint ch;\nint main() {\n");
	tabs++;
	print_switch(tabs,0);
	print_tabs(tabs);
	fprintf(source,"return 0;\n");
	tabs--;
	fprintf(source,"}\n");
}
int main()
{
	int i;
	char str[100];
	sz=1; data[0]=-1; memset(ne[0],0,sizeof(ne[0]));
	//memset(data,-1,sizeof(data));
	for(i=0;i<n;i++)
	{
		sprintf(str,"data%d.in",i);
		in[i]=fopen(str,"r");
		sprintf(str,"data%d.ans",i);
		out[i]=fopen(str,"r");
		doit(i);
	}
	for(i=0;i<n;i++) fclose(in[i]);
	source=fopen("ac_program.cpp","w");
	print();
	for(i=0;i<n;i++) fclose(out[i]);
	fclose(source);
	return 0;
}
