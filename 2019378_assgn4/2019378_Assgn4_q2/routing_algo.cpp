#include "node.h"
#include <iostream>

using namespace std;

void printRT(vector<RoutingNode*> nd){
/*Print routing table entries*/
	for (int i = 0; i < nd.size(); i++) {
	  nd[i]->printTable();
	}
}

void clearMessages(vector<RoutingNode*> *nd){
  for(int i=0;i<nd->size();i++)
    (*nd)[i]->messages.clear();
}

void sendMessages(vector<RoutingNode*> *nd){
  for(int i=0;i<nd->size();i++)
    (*nd)[i]->sendMsg();

}

int getIndex(RoutingNode *node,string s){
  for(int i=0;i< node->mytbl->tbl.size();i++){
    if(s==node->mytbl->tbl[i].dstip)
      return i;
  }
  return -1;

}

bool equal(vector<RoutingEntry> one,vector<RoutingEntry> two){
  if(one.size()!=two.size())
    return false;
  for(int i=0;i<one.size();i++){
    if(one[i].dstip!=two[i].dstip || one[i].nexthop!=two[i].nexthop || one[i].ip_interface!= two[i].ip_interface || one[i].cost!=two[i].cost)
      return false;
  }

  return true;

}

void updateTable(RoutingNode *node,RouteMsg *msg){
  vector<RoutingEntry> table=msg->mytbl->tbl;

  for(int i=0;i<table.size();i++){
    int index=getIndex(node,table[i].dstip);

    if(index==-1){
      int cost=min(16,table[i].cost+1);

      RoutingEntry *entry=new RoutingEntry();
        entry->dstip=table[i].dstip;
        entry->nexthop=msg->from;
        entry->ip_interface=msg->recvip;
        entry->cost=cost;

        node->mytbl->tbl.push_back(*entry);

    }
  else{
      int cost=min(16,table[i].cost+1);
      if(cost < node->mytbl->tbl[index].cost){

        node->mytbl->tbl[index].cost=cost;
        node->mytbl->tbl[index].ip_interface=msg->recvip;
        node->mytbl->tbl[index].nexthop=msg->from;
        
    }
   } 
}
}

void clearRoutingTable(RoutingNode *node){

  node->getTable()->tbl.clear();
  //cout << node->getName() +" ";

  for(int i=0;i<node->interfaces.size();i++){
    
  //  cout << node->interfaces[i].first.getip()+" "<<endl;
    RoutingEntry *entry=new RoutingEntry();
    entry->dstip=node->interfaces[i].first.getip();
    entry->nexthop=node->interfaces[i].first.getip();
    entry->ip_interface=node->interfaces[i].first.getip();
    entry->cost=0;
    node->getTable()->tbl.push_back(*entry);
  }
  //cout<< node->getTable()->tbl.size();

  // if(tableUpdated){
  //   if(node->getName()=="B"){
  //     RoutingEntry *entry=new RoutingEntry();
  //      entry->dstip="10.0.1.3";
  //     entry->nexthop="10.0.1.3";
  //     entry->ip_interface="10.0.1.23";
  //     entry->cost=16;


  //     node->getTable()->tbl.push_back(*entry);
  //   }
  //   else if(node->getName()=="C"){
  //     RoutingEntry *entry=new RoutingEntry();
  //      entry->dstip="10.0.1.23";
  //   entry->nexthop="10.0.1.23";
  //   entry->ip_interface="10.0.1.3";
  //   entry->cost=16;

  //     node->getTable()->tbl.push_back(*entry);

  //   }
  // }
  
}

void updateTblEntry(RoutingNode* B,RoutingNode* C){

  //tableUpdated=true;
  B->tableUpdate=true;
  C->tableUpdate=true;
  for(int i=0;i<B->mytbl->tbl.size();i++){
    if(B->mytbl->tbl[i].dstip=="10.0.1.3")
      B->mytbl->tbl[i].cost=16;
  }

  for(int i=0;i<C->mytbl->tbl.size();i++){
    if(C->mytbl->tbl[i].dstip=="10.0.1.23")
      C->mytbl->tbl[i].cost=16;
  }

}



void routingAlgo(vector<RoutingNode*> nd){
  //Your code here
  bool converged=false;

  while(!converged){
    printRT(nd);
    cout <<endl;
    converged=true;
    clearMessages(&nd);
    sendMessages(&nd);

    for(int i=0;i<nd.size();i++){
      vector<RoutingEntry> copy=nd[i]->getTable()->tbl;
      clearRoutingTable(nd[i]);

      for(int j=0;j<nd[i]->messages.size();j++){

        updateTable(nd[i],nd[i]->messages[j]);
      }

       if(!equal(copy,nd[i]->getTable()->tbl))
      converged=false;

    }

  }
  // converged
  updateTblEntry(nd[1],nd[2]);
  converged=false;

   while(!converged){
    printRT(nd);
    cout<<endl;
    converged=true;
    clearMessages(&nd);
    sendMessages(&nd);

    for(int i=0;i<nd.size();i++){
      vector<RoutingEntry> copy=nd[i]->getTable()->tbl;
      clearRoutingTable(nd[i]);

      for(int j=0;j<nd[i]->messages.size();j++){

        updateTable(nd[i],nd[i]->messages[j]);
      }

       if(!equal(copy,nd[i]->getTable()->tbl))
      converged=false;

    }

  }


  
  


  /*Print routing table entries after routing algo converges */
  printRT(nd);
}


void RoutingNode::recvMsg(RouteMsg *msg) {
  //your code here
  RouteMsg *newmsg=new RouteMsg();
  newmsg->from=msg->from;
  newmsg->recvip=msg->recvip;

  routingtbl *newtbl=new routingtbl();
  newtbl->tbl=msg->mytbl->tbl;
  newmsg->mytbl=newtbl;

  this->messages.push_back(newmsg);

 // cout<<"hello world";


}


/* 
  nodes a and b are stuck in the count of infinity problem, because they keep sharing information 
  about the distance to c until they reach to 16 hops

*/




