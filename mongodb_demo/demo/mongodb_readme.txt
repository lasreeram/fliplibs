/etc/init.d/mongodb status
/var/log/mongodb/mongodb.log
install mongodb using - sudo apt-get install mongodb

commands:

use <dbname>
show collections
db.<collections name>.insert ([ list of json objects here ]);
db.<collection name>.find()
db.<collection name>.find().pretty()
db.<collection name>.remove ( json object containing only the id  )
db.<collection name>.update ( json object containing only the id, json object containing the entire new data  )


C++ Driver
got C++ driver (legacy brach) from Github cloned to ~/mongo_cxx_driver_src.
git clone -b legacy https://github.com/mongodb/mongo-cxx-driver.git
used scons --prefix=/home/hduser/pdir --ssl install

