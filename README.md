This project has a built in serializer that uses the factory design pattern.
it works by initializing a creator template to its bace class and derived class. The downside is that every derived object
has to have a default constructor. the serializer outputs to a json file using the rapidjson library.

the project has several things serialized such as gameobjects, components and even commands.

Multiplayers has only 1 level and Singleplayer has 3. when completing the 3 levels you go to the leaderboard where you can type a name
that will be saved to the leaderboard. 

the box2d library was used for most physics related items like collision, raycasting and triggers. the rigidbody component has an
onenter and onexit event useful for certain interaction between objects like players and enemies.
