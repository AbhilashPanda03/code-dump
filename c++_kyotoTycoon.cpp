#include <ktremotedb.h>

using namespace std;
using namespace kyototycoon;

int main(int argc,char **argv) {
	RemoteDB db;
	if(!db.open("abhilash-HP-ProBook-4330s-mspl")) {
		cerr<<"open error: "<<db.error().name()<<endl;
	}
	if(!db.set("foo","hop") ||
	   !db.set("bar","step") || 
	   !db.set("baz","jump")) {
		cerr<<"set error: "<<db.error().name()<<endl;
	}
	string value;
	if(db.get("foo",&value)) {
		cout<<value<<endl;
	} else {
		cerr<<"get error: "<<db.error().name()<<endl;
	}
	RemoteDB::Cursor* cur=db.cursor();
	cur->jump();
	string ckey, cvalue;
	while (cur->get(&ckey, &cvalue, NULL, true)) {
		cout << ckey << ":" << cvalue << endl;
	}
	delete cur;

	// close the database
	if (!db.close()) {
		cerr << "close error: " << db.error().name() << endl;
	}

	return 0;
}