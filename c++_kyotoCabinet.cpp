#include "crow_all.h"
#include <jsoncpp/json/json.h>
#include <fstream>
#include <bits/stdc++.h>
#include <kchashdb.h>

using namespace std;
using namespace kyotocabinet;

int main() {
	crow::SimpleApp app;
	HashDB db;

	CROW_ROUTE(app,"/")([&db](){
		if(!db.open("casket.kch",HashDB::OWRITER | HashDB::OCREATE)) {
			cerr<<"open error: "<<db.error().name()<<endl;
		}
		return "Successfully connected to DB";
	});

	CROW_ROUTE(app,"/set")([&db](){
		/*ifstream ifs("example_1.json");
		Json::Reader reader;
		Json::Value obj;
		reader.parse(ifs,obj);
		const Json::Value& peoples=obj["peoples"];
		string key,name,age,profession;
		map<string,string> person;
		for(int i=0;i<peoples.size();i++) {
			key="person:"+to_string(i);
			name=peoples[i]["name"].asString();
			age=peoples[i]["age"].asString();
			profession=peoples[i]["profession"].asString();
			person.clear();
			person.insert(pair<string,string>("name",name));
			person.insert(pair<string,string>("age",age));
			person.insert(pair<string,string>("profession",profession));
		}*/
		if(!db.set("foo","hop") ||
	   	   !db.set("bar","step") ||
	       !db.set("baz","jump")) {
		   	cerr<<"set error: "<<db.error().name()<<endl;
		}
		return "Successfull executed command";
	});

	CROW_ROUTE(app,"/get")([&db](){
		string value;
		if(db.get("foo",&value)) {
			cout<<value<<endl;
		} else {
			cerr<<"get error: "<<db.error().name()<<endl;
		}
		return "Successfull executed command";
	});

	CROW_ROUTE(app,"/retrieve")([&db](){
		DB::Cursor* cur=db.cursor();
		cur->jump();
		string ckey,cvalue;
		while(cur->get(&ckey,&cvalue,true)) {
			cout<<ckey<<":"<<cvalue<<endl;
		}
		return "Successfull executed command";
		delete cur;
	});

	CROW_ROUTE(app,"/close")([&db](){
		if(!db.close()) {
			cerr<<"close error: "<<db.error().name()<<endl;
		}
		return "Successfull closed DB";
	});


	app.port(18080).multithreaded().run();	
	
}
