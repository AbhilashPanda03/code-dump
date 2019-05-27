#include "crow_all.h"
#include <hiredis.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

void hmset( redisContext *c, const string &key, const map<string,string> &m )
{
  vector<const char *> argv;
  vector<size_t> argvlen;

  static char cmd[] = "HMSET";
  argv.push_back( cmd );
  argvlen.push_back( sizeof(cmd)-1 );

  argv.push_back( key.c_str() );
  argvlen.push_back( key.size() );

  map<string,string>::const_iterator i;
  for ( i=m.begin(); i!=m.end(); ++i )
  {
    argv.push_back( i->first.c_str() );
    argvlen.push_back( i->first.size() );
    argv.push_back( i->second.c_str() );
    argvlen.push_back( i->second.size() );
  }

  void *r = redisCommandArgv(c, argv.size(), &(argv[0]), &(argvlen[0]) );
  if ( !r )
    throw runtime_error( "Redis error" );
  freeReplyObject( r );
}

int main() {
	crow::SimpleApp app;
	redisContext *c;	

	CROW_ROUTE(app,"/")([&c](){
		c=redisConnect("127.0.0.1",6379);
		if(c==NULL || c->err) {
			if(c) {
				//printf("Error:%s\n",c->errstr);
				return "Error";					
			} else {
				//printf("Can't allocate redis context\n");
				return "Can't allocate redis context";
			}
		} 
		return "Successfully connected to Redis database";
	});

	CROW_ROUTE(app,"/send")([&c](){
		void *reply=redisCommand(c,"SET a bcd");
		return "Successfull executed command";
	});

	CROW_ROUTE(app,"/insert")([&c](){
		//void *reply=redisCommand(c,"SET a bcd");
		//ifstream sample_file("example_1.json",ifstream::binary);
		ifstream ifs("example_1.json");
		//Json::Value sample;
		Json::Reader reader;
		Json::Value obj;
		//sample_file>>sample;
		reader.parse(ifs,obj);
		//cout<<sample;
		//cout<<sample["fruit"];
		const Json::Value& peoples=obj["peoples"];
		//void *reply;		
		string key,name,age,profession;
		map<string,string> person;
		for(int i=0;i<peoples.size();i++) {
			//cout<<"name: "<<peoples[i]["name"].asString()<<endl;
			//cout<<"age: "<<peoples[i]["age"].asUInt()<<endl;
			//cout<<"profession: "<<peoples[i]["profession"].asString()<<endl;
			//cout<<endl;
			//reply=redisCommand(c,"HMSET person:%s name \"%s\" age \"%s\" profession \"%s\"",to_string(i),peoples[i]["name"].asString(),peoples[i]["age"].asString(),peoples[i]["profession"].asString());
			key="person:"+to_string(i);
			name=peoples[i]["name"].asString();
			age=peoples[i]["age"].asString();
			profession=peoples[i]["profession"].asString();
			person.clear();
			person.insert(pair<string,string>("name",name));
			person.insert(pair<string,string>("age",age));
			person.insert(pair<string,string>("profession",profession));
			hmset(c,key,person);
		}
		return "Successfull executed command";
	});

	app.port(18080).multithreaded().run();	
	
}
