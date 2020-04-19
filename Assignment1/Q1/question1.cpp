/**************** library used ************/
#include <bits/stdc++.h>
#include <string>
// #include <filesystem>
#include <math.h>
#include <fstream>

using namespace std;

/*********** variable declared and caontainer used ************/
vector<vector<int> > edges(200);
map <pair<int,int>, pair<int,int> > mp;
map <pair<int,int>, pair<int,int> > mp1;
map <pair<int,int>, pair<int,int> > mp2;
map <pair<int,int>, pair<int,int> > tmpmap;
map <pair<int,int>, int > used_cap1;
map <pair<int,int>, int > used_cap2;
map <pair<int,int>, bool > mp3;
map <pair<int,int>, bool > mp4;
map <pair<int,int>, bool > mp5;
map <pair<int,int>, bool > mp6;


map <pair<int,int>, vector<int> > path1;
map <pair<int,int>, vector<int> > path2;
map <pair<int,int>, vector<int> > path3;
map <pair<int,int>, vector<int> > path4;
map <pair<int,int>, vector<int> > req_map1;
map <pair<int,int>, vector<int> > req_map2;
map <pair<int,int>, int > cost1;
map <pair<int,int>, int > cost2;
map <pair<int,int>, int > cost3;
map <pair<int,int>, int > cost4;
map <pair<int,int>, int > vcid;

int V, e;
int parent[1000];
int distances[1000];
bool includedSet[1000];
int src=0,dest=0;
int no_of_request=0;
int arr[1000][6];
int req[2000][5];
int nodec, edgec;
int accpt=0;
/********************************************/


int mindistancesance()
{
    int mn = INT_MAX, min_ind;

    for(int i = 0; i < V; i++)
    {
        if(includedSet[i] == false && distances[i] <= mn)
            mn = distances[i], min_ind = i;
    }

    return min_ind;
}


/************ storing the paths **************/
void path_one_print( int j)
{
    if(parent[j]==-1)
        return ;
   
    path_one_print(parent[j]);
    path1[make_pair(src,dest)].push_back(j);   // storing nodes
   
}

void path_two_print( int j)
{
    if(parent[j]==-1)
        return ;
    path_two_print( parent[j]);
    path2[make_pair(src,dest)].push_back(j); // storing nodes
    
}

void path_three_print( int j)
{
    if(parent[j]==-1)
        return ;
    path_three_print( parent[j]);
    path3[make_pair(src,dest)].push_back(j);  // storing nodes
}

void path_four_print( int j)
{
    if(parent[j]==-1)
        return ;
    path_four_print( parent[j]);
    path4[make_pair(src,dest)].push_back(j);  // storing nodes
}

/*********************************************/


/*************** find all the path from given source ************/
int solution1() 
{ 

    // loop for all the paths
    for (int i = 0; i < V; i++) 
    { 
        dest = i;

        if(src != dest && mp3.find(make_pair(src,dest)) == mp3.end())
        {
            cost1[make_pair(src,i)]=distances[i];   // finding the costs
            cost1[make_pair(i,src)]=distances[i];   // finding the costs
            // printf("\n%d -> %d \t\t %d\t\t%d ", src, i, distances[i], src); 
            path1[make_pair(src,dest)].push_back(src);
        
            
            path_one_print(i);    
            vector<int> vt = path1[make_pair(src,dest)];     
            reverse(vt.begin(),vt.end());
            path1[make_pair(dest,src)] = vt;
            mp3[make_pair(src,dest)] = true;
            mp3[make_pair(dest,src)] = true;
            
       }
        
    } 

    return 0;
}

int solution2( ) 
{ 
    
        cost2[make_pair(src,dest)]=distances[dest];
        cost2[make_pair(dest,src)]=distances[dest];
        // printf("\n%d -> %d \t\t %d\t\t%d ", 
                      // src, dest, distances[dest], src); 
        path2[make_pair(src,dest)].push_back(src);
        
      
        path_two_print(dest); 
        vector<int> vt = path2[make_pair(src,dest)];
        vt = path2[make_pair(src,dest)];

        reverse(vt.begin(),vt.end());
        path2[make_pair(dest,src)] = vt;
        mp4[make_pair(src,dest)] = true;
        mp4[make_pair(dest,src)] = true; 

    return 0;
}  


int printSolution3() 
{ 
   
    for (int i = 0; i < V; i++) 
    { 
        dest = i;

        if(src != dest && mp5.find(make_pair(src,dest)) == mp5.end())
        {
            cost3[make_pair(src,i)]=distances[i];
            cost3[make_pair(i,src)]=distances[i];
            // printf("\n%d -> %d \t\t %d\t\t%d ", src, i, distances[i], src); 
            path3[make_pair(src,dest)].push_back(src);
        
            
            path_three_print(i); 
          
            vector<int> vt = path3[make_pair(src,dest)];
           
            
            reverse(vt.begin(),vt.end());
            path3[make_pair(dest,src)] = vt;
          
            mp5[make_pair(src,dest)] = true;
            mp5[make_pair(dest,src)] = true;
            
       }

 
        
    } 

    return 0;
}

int printSolution4( ) 
{ 
        
        cost4[make_pair(src,dest)]=distances[dest];
        cost4[make_pair(dest,src)]=distances[dest];
        // printf("\n%d -> %d \t\t %d\t\t%d ", 
        //               src, dest, distances[dest], src); 
        path4[make_pair(src,dest)].push_back(src);
        
    
        path_four_print(dest); 
        vector<int> vt = path4[make_pair(src,dest)];
        vt = path4[make_pair(src,dest)];

        reverse(vt.begin(),vt.end());
        path4[make_pair(dest,src)] = vt;
        mp6[make_pair(src,dest)] = true;
        mp6[make_pair(dest,src)] = true; 

    return 0;
}  




/******************** applying djilstra algorithms for Dist option ************/
void djikshtra_delay(int s, int flag1)
{
  
   
    for (int i = 0; i < V; i++) 
    { 
        parent[i] = -1; 
        distances[i] = INT_MAX; 
        includedSet[i] = false; 
    } 

   
    distances[src]=0;
    for(int count = 0;count < V-1;count++)
    {
        int u = mindistancesance();

        includedSet[u] = true;
        for(int y = 0; y < V;y++)
        {
            if(!includedSet[y] && mp.find({u,y})!= mp.end()  && distances[u] + mp[make_pair(u,y)].first < distances[y])
            {
                parent[y] = u;
                distances[y] = distances[u] + mp[make_pair(u,y)].first;

            }
        }
    }

  
   if(flag1)
   solution1(); 
  else solution2();

}
/*****************************************/



/******************** applying djilstra algorithms for hop option ************/
void djikshtra_edge(int s, int flag1)
{
  
    for (int i = 0; i < V; i++) 
    { 
        parent[i] = -1; 
        distances[i] = INT_MAX; 
        includedSet[i] = false; 
    } 

    distances[src]=0;
 
    for(int count = 0;count < V-1;count++)
    {
        int u = mindistancesance();

        includedSet[u] = true;
       
        for(int y = 0; y < V;y++)
        {
            if(!includedSet[y] && mp1.find({u,y})!= mp1.end()  && 
                distances[u] + mp1[make_pair(u,y)].first < distances[y])
            {
                parent[y] = u;
                // cout<<parent[y]<<endl;
                distances[y] = distances[u] + mp1[make_pair(u,y)].first;

            }
        }
    }

   if(flag1)
   printSolution3(); 
  else printSolution4();
}
/***************************************************************************/

void input(char const *argv[])
{
	string word;
	fstream file;
	string filename;
	filename = argv[1];

	file.open(filename.c_str());
	
	file >> word;
	nodec = stoi(word);
	file >> word;
	edgec = stoi(word);
	V = nodec;
	// cout<<nodec<<" "<<edgec<<endl;
    
    for(int i=0;i<edgec;i++)
    {
    	int j=0;
	    while(j<5)
		{
			file >> word;
			arr[i][j] = stoi(word);
			// cout<<word<<" ";
			j++;
		}
		// cout<<endl;
    }
    file.close();

    string filename2 = argv[2];
    file.open(filename2.c_str());
    int n ;
    word = "";
    file >> word;
    n = stoi(word);
    no_of_request=n;
   
   
    for(int i=0;i<n;i++)
    {
    	int j=0;
    	while(j<5)
    	{
           file>>word;
           req[i][j]= stoi(word);
           j++;
    	}
    }

}


/*************** making graph ******************/
void graph()
{
    for(int i=0;i<edgec;i++)
    {
    	pair<int,int> p,q,r,t;
    	edges[arr[i][0]].push_back(arr[i][1]);
    	edges[arr[i][1]].push_back(arr[i][0]);
    	p = make_pair(arr[i][2],arr[i][3]);
    	q = make_pair(arr[i][0],arr[i][1]);
    	r = make_pair(arr[i][1],arr[i][0]);
        t = make_pair(1,arr[i][3]);
    	mp[q] = p;
        mp1[q] = p;
    	mp[r] = p;
        mp1[r] =p;
        used_cap1[q]=0;
        used_cap1[r]=0;
        used_cap2[q]=0;
        used_cap2[r]=0;
        vcid[q] = 0;
        vcid[r]=0;
    }

    mp2 = mp;

    for(auto itr = mp1.begin();itr != mp1.end();itr++)
    {
        itr->second.first = 1;
    }

    tmpmap = mp1;

   
    
    for(int i=0;i<no_of_request;i++)
    {
        pair<int,int> p;
        p = make_pair(req[i][0],req[i][1]);
        req_map1[p].push_back(req[i][2]);
        req_map1[p].push_back(req[i][3]);
        req_map1[p].push_back(req[i][4]);

    }

}


void removefunc(int a, int b)
{
    vector<int> vect;
    vect = path1[make_pair(a,b)];
    for(int i=0;i<vect.size()-1;i++)
    {
       mp.erase(make_pair(vect[i],vect[i+1]));
       mp.erase(make_pair(vect[i+1],vect[i]));
    }
}

void removefunc2(int a, int b)
{
    vector<int> vect;
    vect = path3[make_pair(a,b)];
    for(int i=0;i<vect.size()-1;i++)
    {
       mp1.erase(make_pair(vect[i],vect[i+1]));
       mp1.erase(make_pair(vect[i+1],vect[i]));
    }
}

void optimistic_approach_dist()
{
    ofstream fout,fout2; 
    ofstream fout1;
    fout.open("routingtable.txt"); 
    fout1.open("forwarding.txt");
    fout2.open("pathfile.txt");
    cout<<"optimistic_approach1"<<endl;
    int accepted=0;
   
    cout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout1<<"Router’s ID"<<"\t|\tID of Incoming Port"<<"\t|\tVCID"<<"\t|\tID of Outgoing Port"<<"\t|\tVCID"<<endl;
   for(int i=0;i<no_of_request;i++)
   {
      cout<<req[i][1]<<" \t|\t";
      fout<<req[i][1]<<" \t|\t";
     int tmp1 = req[i][3] + ((req[i][4]-req[i][2])/4);
     int Bi_equ = min(req[i][4],tmp1);
     // applying Optimistic Approach

     vector<int> vt1, vt2, vt3;
     vt1 = path1[make_pair(req[i][0],req[i][1])];
     vt2 =  path2[make_pair(req[i][0],req[i][1])];

     bool flag1=true,flag2=true;
     int tmp2,tmp3;

     // cheking for first paths
     for(int j=0;j<vt1.size()-1;j++)
     {
       
        tmp2 = mp[make_pair(vt1[j],vt1[j+1])].second - used_cap1[make_pair(vt1[j],vt1[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

     // path found then adding cost and printing in files
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt1.size()-1;j++)
         {
           cout<<vt1[j]<<" ";
           fout<<vt1[j]<<" ";
           if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt1[j],vt1[j+1])]++;
                vcid[make_pair(vt1[j+1],vt1[j+2])]++;
                fout1<<vt1[j+1]<<" \t|\t"<<vt1[j]<<"\t|\t"<<vcid[make_pair(vt1[j],vt1[j+1])]<<"\t|\t"<<vt1[j+1]<<"\t|\t"<<vcid[make_pair(vt1[j+1],vt1[j+2])]<<endl;
            }
            used_cap1[make_pair(vt1[j],vt1[j+1])] = used_cap1[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            used_cap1[make_pair(vt1[j+1],vt1[j])] = used_cap1[make_pair(vt1[j+1],vt1[j])] + Bi_equ;
            

           
         }
         cout<<vt1[vt1.size()-1]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt1[vt1.size()-1]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
         continue;
     }
    

     flag1 =true;
     //cheking for second paths
     for(int j=0;j<vt2.size()-1;j++)
     {
        tmp2 = mp[make_pair(vt2[j],vt2[j+1])].second - used_cap1[make_pair(vt2[j],vt2[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

    //second path find then adding cost
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt2.size()-1;j++)
         {

            cout<<vt2[j]<<" ";
            fout<<vt2[j]<<" ";
             if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt2[j],vt2[j+1])]++;
                vcid[make_pair(vt2[j+1],vt2[j+2])]++;
                fout1<<vt2[j+1]<<" \t|\t"<<vt2[j]<<"\t|\t"<<vcid[make_pair(vt2[j],vt2[j+1])]<<"\t|\t"<<vt2[j+1]<<"\t|\t"<<vcid[make_pair(vt2[j+1],vt2[j+2])]<<endl;
            }
            used_cap1[make_pair(vt2[j],vt2[j+1])] = used_cap1[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
            used_cap1[make_pair(vt2[j+1],vt2[j])] = used_cap1[make_pair(vt2[j+1],vt2[j])] + Bi_equ;
         }
         
         cout<<vt2[vt2.size()-1]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt2[vt2.size()-1]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
        
         continue;
     }
     
     int zero=0;
      // printing in file as wel as console if connection is not acceted
     cout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
     fout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;

   }

    fout2<<no_of_request<<" "<<accepted<<endl;
    accpt = accepted;
   fout.close();
   fout1.close();
   fout2.close();
}

void optimistic_approach_hop()
{

    ofstream fout,fout2; 
    ofstream fout1;
    fout.open("routingtablef.txt"); 
    fout1.open("forwarding.txt");
    fout2.open("pathfile.txt");
    cout<<"optimistic_approach2"<<endl;
    int accepted=0;
   
    cout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout1<<"Router’s ID"<<"\t|\tID of Incoming Port"<<"\t|\tVCID"<<"\t|\tID of Outgoing Port"<<"\t|\tVCID"<<endl;
   for(int i=0;i<no_of_request;i++)
   {
      cout<<req[i][1]<<" \t|\t";
      fout<<req[i][1]<<" \t|\t";
     
     int tmp1 = req[i][3] + ((req[i][4]-req[i][2])/4);
     int Bi_equ = min(req[i][4],tmp1);
    // applying Optimistic Approach:


     vector<int> vt1, vt2, vt3;
     vt1 = path3[make_pair(req[i][0],req[i][1])];
     vt2 =  path4[make_pair(req[i][0],req[i][1])];

     bool flag1=true,flag2=true;
     int tmp2,tmp3;
     
     // cheking for first paths
     for(int j=0;j<vt1.size()-1;j++)
     {
        
        tmp2 = mp[make_pair(vt1[j],vt1[j+1])].second - used_cap1[make_pair(vt1[j],vt1[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

    
     // path found then adding cost and printing in files
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt1.size()-1;j++)
         {
           cout<<vt1[j]<<" ";
           fout<<vt1[j]<<" ";
           if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt1[j],vt1[j+1])]++;
                vcid[make_pair(vt1[j+1],vt1[j+2])]++;
                fout1<<vt1[j+1]<<" \t|\t"<<vt1[j]<<"\t|\t"<<vcid[make_pair(vt1[j],vt1[j+1])]<<"\t|\t"<<vt1[j+1]<<"\t|\t"<<vcid[make_pair(vt1[j+1],vt1[j+2])]<<endl;
            }
            used_cap1[make_pair(vt1[j],vt1[j+1])] = used_cap1[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            used_cap1[make_pair(vt1[j+1],vt1[j])] = used_cap1[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            sum += mp[make_pair(vt1[j],vt1[j+1])].first;
         }
         cout<<vt1[vt1.size()-1]<<" \t|\t"<<cost3[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt1[vt1.size()-1]<<" \t|\t"<<cost3[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;

         continue;
     }
      

      // cheking for second paths
     flag1 =true;
     for(int j=0;j<vt2.size()-1;j++)
     {
        tmp2 = mp[make_pair(vt2[j],vt2[j+1])].second - used_cap1[make_pair(vt2[j],vt2[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

     // path found then adding cost and printing in files
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt2.size()-1;j++)
         {

            cout<<vt2[j]<<" ";
            fout<<vt2[j]<<" ";
             if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt2[j],vt2[j+1])]++;
                vcid[make_pair(vt2[j+1],vt2[j+2])]++;
                fout1<<vt2[j+1]<<" \t|\t"<<vt2[j]<<"\t|\t"<<vcid[make_pair(vt2[j],vt2[j+1])]<<"\t|\t"<<vt2[j+1]<<"\t|\t"<<vcid[make_pair(vt2[j+1],vt2[j+2])]<<endl;
            }
            used_cap1[make_pair(vt2[j],vt2[j+1])] = used_cap1[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
            used_cap1[make_pair(vt2[j+1],vt2[j])] = used_cap1[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
         }
         
         cout<<vt2[vt2.size()-1]<<" \t|\t"<<cost4[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt2[vt2.size()-1]<<" \t|\t"<<cost4[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
         
         continue;
     }
     
         int zero=0;
          // printing in file as wel as console if connection is not acceted
         cout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
         fout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
   
   }

       fout2<<no_of_request<<" "<<accepted<<endl;
         accpt = accepted;
       fout.close();
       fout1.close();
       fout2.close();

}

void pessimistic_Approach_dist()
{

 ofstream fout,fout2; 
    ofstream fout1;
    
    fout.open("routingtablef.txt"); 
    fout1.open("forwarding.txt");
    fout2.open("pathfile.txt");
    cout<<"pessimistic_Approach1"<<endl;
    int accepted=0;
   
    cout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout1<<"Router’s ID"<<"\t|\tID of Incoming Port"<<"\t|\tVCID"<<"\t|\tID of Outgoing Port"<<"\t|\tVCID"<<endl;
   for(int i=0;i<no_of_request;i++)
   {
      cout<<req[i][1]<<" \t|\t";
      fout<<req[i][1]<<" \t|\t";
     
     int tmp1 = req[i][3] + ((req[i][4]-req[i][2])/4);
     int Bi_equ = req[i][4];
     vector<int> vt1, vt2, vt3;
     vt1 = path1[make_pair(req[i][0],req[i][1])];
     vt2 =  path2[make_pair(req[i][0],req[i][1])];

     bool flag1=true,flag2=true;
     int tmp2,tmp3;
      // cheking for first paths
     for(int j=0;j<vt1.size()-1;j++)
     {
        // cout<<"inner loop 1"<<endl;
        tmp2 = mp[make_pair(vt1[j],vt1[j+1])].second - used_cap2[make_pair(vt1[j],vt1[j+1])];
        
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

     // path found then adding cost and printing in files
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt1.size()-1;j++)
         {
           cout<<vt1[j]<<" ";
           fout<<vt1[j]<<" ";
           if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt1[j],vt1[j+1])]++;
                vcid[make_pair(vt1[j+1],vt1[j+2])]++;
                fout1<<vt1[j+1]<<" \t|\t"<<vt1[j]<<"\t|\t"<<vcid[make_pair(vt1[j],vt1[j+1])]<<"\t|\t"<<vt1[j+1]<<"\t|\t"<<vcid[make_pair(vt1[j+1],vt1[j+2])]<<endl;
            }
            used_cap2[make_pair(vt1[j],vt1[j+1])] = used_cap2[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            used_cap2[make_pair(vt1[j+1],vt1[j])] = used_cap2[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            sum += mp[make_pair(vt1[j],vt1[j+1])].first;

            
         }
         cout<<vt1[vt1.size()-1]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt1[vt1.size()-1]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
      
         continue;
     }
      
     flag1 =true;
     // cheking for second paths
     for(int j=0;j<vt2.size()-1;j++)
     {
        tmp2 = mp[make_pair(vt2[j],vt2[j+1])].second - used_cap2[make_pair(vt2[j],vt2[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }
   
   // path found then adding cost and printing in files
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt2.size()-1;j++)
         {

            cout<<vt2[j]<<" ";
            fout<<vt2[j]<<" ";
             if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt2[j],vt2[j+1])]++;
                vcid[make_pair(vt2[j+1],vt2[j+2])]++;
                fout1<<vt2[j+1]<<" \t|\t"<<vt2[j]<<"\t|\t"<<vcid[make_pair(vt2[j],vt2[j+1])]<<"\t|\t"<<vt2[j+1]<<"\t|\t"<<vcid[make_pair(vt2[j+1],vt2[j+2])]<<endl;
            }
            used_cap2[make_pair(vt2[j],vt2[j+1])] = used_cap2[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
            used_cap2[make_pair(vt2[j+1],vt2[j])] = used_cap2[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
         }
        
         cout<<vt2[vt2.size()-1]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt2[vt2.size()-1]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
         
         continue;
     }
     
         int zero=0;
          // printing in file as wel as console if connection is not acceted
         cout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
         fout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
        // cout<<"connection is not accepted"<<endl;
    


   }

       fout2<<no_of_request<<" "<<accepted<<endl;
         accpt = accepted;
       fout.close();
       fout1.close();
       fout2.close();
       // closing all the opened files

}

void pessimistic_Approach_hop()
{
    ofstream fout,fout2; 
    ofstream fout1;
    fout.open("routingtablef.txt"); 
    fout1.open("forwarding.txt");
    fout2.open("pathfile.txt");
    cout<<"pessimistic_Approach2"<<endl;
    int accepted=0;
   
    cout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout<<"destination Node"<<"\t|\tPath"<<"\t|\tPath delay"<<"\t|\tCost"<<endl;
    fout1<<"Router’s ID"<<"\t|\tID of Incoming Port"<<"\t|\tVCID"<<"\t|\tID of Outgoing Port"<<"\t|\tVCID"<<endl;
   for(int i=0;i<no_of_request;i++)
   {
      cout<<req[i][1]<<" \t|\t";
      fout<<req[i][1]<<" \t|\t";
     int tmp1 = req[i][3] + ((req[i][4]-req[i][2])/4);
     int Bi_equ = req[i][4];
     vector<int> vt1, vt2, vt3;
     vt1 = path3[make_pair(req[i][0],req[i][1])];
     vt2 =  path4[make_pair(req[i][0],req[i][1])];

     bool flag1=true,flag2=true;
     int tmp2,tmp3;
     
     for(int j=0;j<vt1.size()-1;j++)
     {
        // cout<<"inner loop 1"<<endl;
        tmp2 = mp[make_pair(vt1[j],vt1[j+1])].second - used_cap2[make_pair(vt1[j],vt1[j+1])];
       
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }

     // cheking for first paths
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt1.size()-1;j++)
         {
           cout<<vt1[j]<<" ";
           fout<<vt1[j]<<" ";
           if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt1[j],vt1[j+1])]++;
                vcid[make_pair(vt1[j+1],vt1[j+2])]++;
                fout1<<vt1[j+1]<<" \t|\t"<<vt1[j]<<"\t|\t"<<vcid[make_pair(vt1[j],vt1[j+1])]<<"\t|\t"<<vt1[j+1]<<"\t|\t"<<vcid[make_pair(vt1[j+1],vt1[j+2])]<<endl;
            }
            used_cap2[make_pair(vt1[j],vt1[j+1])] = used_cap2[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            used_cap2[make_pair(vt1[j+1],vt1[j])] = used_cap2[make_pair(vt1[j],vt1[j+1])] + Bi_equ;
            sum += mp[make_pair(vt1[j],vt1[j+1])].first;

            
         }
         cout<<vt1[vt1.size()-1]<<" \t|\t"<<cost3[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt1[vt1.size()-1]<<" \t|\t"<<cost3[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost1[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
         
         continue;
     }
      
     flag1 =true;
       // cheking for second paths
     for(int j=0;j<vt2.size()-1;j++)
     {
        tmp2 = mp[make_pair(vt2[j],vt2[j+1])].second - used_cap2[make_pair(vt2[j],vt2[j+1])];
        if(Bi_equ > tmp2)
        {
            flag1=false;
            break;
        }
     }
     
     if(flag1)
     {
        int sum=0;
         for(int j=0;j<vt2.size()-1;j++)
         {

            cout<<vt2[j]<<" ";
            fout<<vt2[j]<<" ";
             if(j+2!=(vt1.size()))
            {
                vcid[make_pair(vt2[j],vt2[j+1])]++;
                vcid[make_pair(vt2[j+1],vt2[j+2])]++;
                fout1<<vt2[j+1]<<" \t|\t"<<vt2[j]<<"\t|\t"<<vcid[make_pair(vt2[j],vt2[j+1])]<<"\t|\t"<<vt2[j+1]<<"\t|\t"<<vcid[make_pair(vt2[j+1],vt2[j+2])]<<endl;
            }
            used_cap2[make_pair(vt2[j],vt2[j+1])] = used_cap2[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
            used_cap2[make_pair(vt2[j+1],vt2[j])] = used_cap2[make_pair(vt2[j],vt2[j+1])] + Bi_equ;
         }
         
         cout<<vt2[vt2.size()-1]<<" \t|\t"<<cost4[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         fout<<vt2[vt2.size()-1]<<" \t|\t"<<cost4[make_pair(req[i][0],req[i][1])]<<" \t|\t"<<cost2[make_pair(req[i][0],req[i][1])]<<endl;
         accepted++;
        
         continue;
     }
     
     int zero=0;
     // printing in file as wel as console if connection is not acceted
     cout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;
     fout<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<"\t|\t"<<"NOT ACC"<<endl;

   }

    fout2<<no_of_request<<" "<<accepted<<endl;
      accpt = accepted;
   fout.close();
   fout1.close();
   fout2.close();


}


/************** main function **************/

int main(int argc, char const *argv[])
{
	

	input(argv);
    graph();
    
    bool flag1=true;

    /*********** path finding distwise using original weight *********/
    for(int i=0;i<V;i++)
    {
        flag1=true;
        src=i;
        djikshtra_delay(i,flag1);
        for(int j=0;j<V;j++)
        {
        
         if(j != i && mp4.find(make_pair(i,j)) == mp4.end())
         {
            dest = j;
            
            removefunc(i,j);
            flag1 = false;
            // cout<<"second"<<i<<" "<<j<<endl;
            djikshtra_delay(i,false);
         }

         mp = mp2;
        }
    }

    /*********** path finding hopwise using giving weight 1 to each edges *********/
    for(int i=0;i<V;i++)
    {
        flag1=true;
        src=i;
        djikshtra_edge(i,flag1);
        for(int j=0;j<V;j++)
        {
        
         if(j != i && mp6.find(make_pair(i,j)) == mp6.end())
         {
            dest = j;
            
            removefunc2(i,j);
            flag1 = false;
            djikshtra_edge(i,false);
         }

         mp1 = tmpmap;
        }
    }

/************* options for flags ***********/
    int op1=0,op2=0;    //op1=0 means dist op1=1 hop
    // op2=0 means optimistic
    //op2 = 1 means pessimistic
    string st = argv[7];
    if(st=="dist")
        op1=0;
    else op1 = 1;

    if(stoi(argv[9])==0)
        op2 = 0;
    else op2 = 1;

   
    if(op1==0 && op2==0)
    {
        optimistic_approach_dist();
        cout<<"optimistic_approach_dist"<<endl;
    }

    if(op1==0 && op2==1)
    {
        pessimistic_Approach_dist();
        cout<<"pessimistic_Approach_dist"<<endl;
    }

    if(op1==1 && op2==0)
    {
       
        optimistic_approach_hop();
        cout<<"optimistic_approach_hop"<<endl;
    }
    
    if(op1==1 && op2==1)
    {
        pessimistic_Approach_hop();
         cout<<"pessimistic_Approach_hop"<<endl;
    }
     
      cout<<"flag options "<<op1<<" "<<op2<<endl;
      cout<<"No of accepted connection "<<accpt<<" out of "<<no_of_request<<endl;
     cout<<"programm is executed"<<endl;
	return 0;
}


