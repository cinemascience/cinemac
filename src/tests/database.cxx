#include "CinDatabase.h"

int main() 
{
    // report
    std::cout << "Running Cinema Database test ..." << std::endl;
    
    // create a cinema database
    CinDatabase db;
    db.setPath("example.cdb");

    // add some parameters
    db.addParameter("first");
    db.addParameter("second");
    db.addParameter("third");
    db.addParameter("fourth");

    std::map<std::string, std::string> art = {{"first", "f_01"}, {"second", "s_01"}, {"third", "t_01"}};
    db.addArtifact( art );
    art = {{"first", "f_02"}, {"second", "s_02"}, {"third", "t_02"}};
    db.addArtifact( art );

    std::map<std::string, std::string> results;
    db.getArtifact( 0, results );

    // check the database
    db.debugPrint();

    return 1;
}
