/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

/** \mainpage

A Rube Goldberg machine, contraption, invention, device, or apparatus is a deliberately over-engineered or overdone machine that performs a very simple task in a very complex fashion, usually including a chain reaction.<br>
We have developed a machine that ends up sliding a gate. Seems simple, right?<br>
Have a look at the initial contraption that we built below.<br>

<img src="../state_00.png">

*/

#include "cs296_base.hpp"
#include "render.hpp"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "GL/freeglut.h"
#endif

#include <cstring>
using namespace std;

#include "dominos.hpp"

namespace cs296
{
/// creates a see saw system which includes a triangular wedge and a bar on top of it.
/// plank_length and plank_breadth are parameters given to the function which denote half of length and width of the plank.
/// x_pos and y_pos are parameters which give position of centre of the system.
void dominos_t::see_saw_create(float x_pos, float y_pos, float plank_length, float plank_breadth)
{

   b2Body* sbody;/// sbody is a b2Body* variable which represents the wedge
   b2PolygonShape poly;/// poly is a polygon shape we will create of type b2Polygonshape 
   b2Vec2 vertices[3];/// create an 2D array using b2Vec2 of size 3 which will store coordinates of the vertices of the polygon poly
   /** store the positions of the 3 vertices. (-1,0) , (1,0 ) and (0,1.5)*/
   vertices[0].Set(-1,0);
   vertices[1].Set(1,0);
   vertices[2].Set(0,1.5);
   poly.Set(vertices, 3);///create a triangular shape called poly with the 3 vertices as specified above
   b2FixtureDef wedgefd;/// define a b2FixtureDef variable wedgefd to create a fixture for sbody
   wedgefd.shape = &poly;///set its shape to poly defined above
   wedgefd.density = 2.0f;/// set its density to 2
   wedgefd.friction = 0.0f;/// set its friction to 0 
   wedgefd.restitution = 0.0f;/// set its restitution to 0
   b2BodyDef wedgebd;/// define a b2BodyDef variable wedgebd which will be used to create the actual wedge
   wedgebd.position.Set(x_pos, y_pos);/// let its position be (x_pos,y_pos) in world coordinates
   sbody = m_world->CreateBody(&wedgebd);/// create the body using wedgebd
   sbody->CreateFixture(&wedgefd);/// add the fixture wedgefd to sbody  ( the vertices of wedgefd will have coordinates wrt sbody )

	///<br>
    /// <b>The plank on top of the wedge</b>
    //The plank on top of the wedge
    b2PolygonShape shape;/// shape is a polygon shape we will create of type  b2PolygonShape
    shape.SetAsBox(plank_length, plank_breadth);/// it is a 2*plank_length X 2*plank_breadth rectangle 
    b2BodyDef bd2;/// define a b2BodyDef variable bd2 which will be used to create the plank
    bd2.position.Set(x_pos, y_pos + 3);/// let its position be (x_pos,y_pos+3) in world coordinates i.e it touches the upper vertex of wedge.
    bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
    b2Body* body = m_world->CreateBody(&bd2);/// create body ( type  b2Body) using bd2
    b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable fd2 to create a fixture for body
    fd2->density = 0.5f;/// set its density to 0.5
    fd2->shape = new b2PolygonShape;
    fd2->shape = &shape;/// set its shape to the shape variable defined above
    body->CreateFixture(fd2); /// add the fixture fd2 to body
	
	///<br> 
    /** <b>create a revolute joint at the point where the wedge and the plank meet for the see-saw.</b>*/
    b2RevoluteJointDef jd;///define a revolute (revolving) joint jd of type b2RevoluteJointDef
    b2Vec2 anchor;/// create a 2d array using b2vec2 called anchor to store location of anchor of point
    anchor.Set(x_pos, y_pos+3);/// set coordinates of anchor to (x_pos,y_pos+3)
    jd.Initialize(sbody, body, anchor);/// initialise jd : define the 2 bodies (sbody and body) and location of common anchor
    jd.enableLimit = true;/// limit the rotation of the joint
  	jd.lowerAngle = -1;
  	jd.upperAngle =  1;
    m_world->CreateJoint(&jd);/// create the joint using jd

}

  //! This is the constructor for dominos_t class. 
  /**	It creates all the elements required in the Box2D simulation. <br>
  We have the following elements defined : <br>
  <ol> 
  <li> Ground </li>
  <li> Various horizontal and inclined shelves </li>
  <li> Dominos at the top : 5 , spherical ball after these dominos </li>
  <li> Pendulum  that knocks down the dominos </li>
  <li> The pulley system near the top
  		<ul> <li> An open box</li>
  			<li> Bar</li> 
  		</ul></li>
  <li> The see-saw system near the top
  		<ul>
  		<li> Triangular wedge </li>
  		<li> Plank on top of wedge</li>
  		<li> 2 balls on either end of plank</li>
  		<li> Vertical obstacle at centre of plank</li>  
  		</ul> </li>
  <li> Balls on platform above pulley system near the bottom</li>
  <li> The pulley system near the bottom
  		<ul> <li> An open box</li>
  			<li> A domino</li> 
  		</ul></li>	
  <li> Unstable Dominos System</li>		
  <li> Revolving platform</li>
  <li> System of see saws at lower right corner</li>			
  <li> Sliding Gate </li>
  <li> Balls and Hinges near the bottom </li>
  </ol>*/		

  dominos_t::dominos_t()
  {
    /// <br> 
    /// 1. <b>Ground</b>
    //Ground
    b2Body* b1;/// b1 is a b2Body* variable which represents the body (for the ground).
    {
      /**shape is a line segment (b2Edgeshape) of length 92 with end points (-46,0) and (46,0)*/
      b2EdgeShape shape;
      shape.Set(b2Vec2(-46.0f, 0.0f), b2Vec2(46.0f, 0.0f));
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static (by default).
      b1 = m_world->CreateBody(&bd);/// create the body b1 using bd
      b1->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
    }
      
     ///  
    ///<br> 2. <b>Various horizontal and inclined shelves</b>
    /// <br> 2. a) <b> Top horizontal shelf </b>
    {
    	/**shape is a line segment (b2Edgeshape) of length 26 with end points (-41.0f,37.5f) and (-15.0f,37.5f)*/
      b2EdgeShape shape;
      shape.Set(b2Vec2(-41.0f,37.5f) , b2Vec2(-15.0f,37.5f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static (by default).
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd (represents the shelf).
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
    }

	/// 
	/// <br> 2. b)<b> The horizontal shelf just below the open box (connected to the pulley near the top)</b>
    {
	/**shape is a line segment (b2Edgeshape) of length 6 with end points (-13.0f,26.0f) and (-7.0f,26.0f)*/
	b2EdgeShape shape;
      shape.Set(b2Vec2(-13.0f,26.0f) , b2Vec2(-7.0f,26.0f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static (by default)
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density	
    }
    
    ///
    /// <br>2. c) <b> Inclined shelf connected to the topmost horizontal shelf</b>
    {
    	/**shape is a 4 X 0.02 rectangle placed at (1.414f,-1.414f) wrt the body it will be attached to, and it is rotated by -0.707 radians in counter clockwise direction.     
    	*/
      b2PolygonShape shape;
      shape.SetAsBox(2.0f, 0.01f, b2Vec2(1.414f,-1.414f), -0.707f);
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static (by default)
      bd.position.Set(-15.0f, 37.5f);/// set the position of the body as (-15.0f, 37.5f)
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
    }
    
    //other platforms
    {
      ///
      ///<br> 2. d)<b> Horizontal shelf below the see saw on the top </b> 
    	/**shape is a line segment (b2Edgeshape) of length 24 with end points (4.0f,23.0f) and (30.0f,23.0f)*/
    	b2EdgeShape shape;
      shape.Set(b2Vec2(4.0f,23.0f) , b2Vec2(30.0f,23.0f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static(by default).
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
      
      // inclined platform 
      /// <br>2. e) <b> The inclined shelf connected to the horizontal shelf below the top see saw. </b>
      /**set shape to a line segment(b2Edgeshape) from (4.0f,23.0f) to (0.0f,18.0f)
      */
      shape.Set(b2Vec2(4.0f,23.0f) , b2Vec2(0.0f,18.0f));
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the existing body with the new shape definition and having 0 density.
      
      ///<br> 2. f) <b>Horizontal shelf near the bottom which is just below the sequence of see saws</b>
      /**set shape to a line segment(b2Edgeshape) from (-7.0f,6.0f)  to (38.0f,6.0f)*/
      shape.Set(b2Vec2(-7.0f,6.0f) , b2Vec2(38.0f,6.0f));
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the existing body with the new shape definition and having 0 density.
      
      ///<br> 2. g) <b>Vertical shelf next to the pulley near the bottom</b>
      /**set shape to a line segment(b2Edgeshape) from (-7.0f,6.0f)  to (-7.0f,11.0f)*/
      shape.Set(b2Vec2(-7.0f,6.0f) , b2Vec2(-7.0f,11.0f));
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the existing body with the new shape definition and having 0 density.
      
      ///<br> 2. h) <b>Horizontal shelf just above the pulley near the bottom</b>
      /**set shape to a line segment(b2Edgeshape) from (-7.0f,11.0f)  to (-15.0f,11.0f)*/
      shape.Set(b2Vec2(-7.0f,11.0f) , b2Vec2(-15.0f,11.0f));
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the existing body with the new shape definition and having 0 density.
      
      ///<br> 2. i) <b>Inclined platform at the bottom right corner</b>
      /**set shape to a line segment(b2Edgeshape) from (40.0f,0.0f)  to (46.0f,8.0f)*/
      shape.Set(b2Vec2(40.0f,0.0f) , b2Vec2(46.0f,8.0f));
      ground->CreateFixture(&shape, 0.0f); /// add a fixture to the existing body with the new shape definition and having 0 density.
      
      ///<br> 2. j) <b>Horizontal shelf below the open box of the pulley system near the bottom. </b>
      /**set shape to a line segment(b2Edgeshape) from (-20.0f,3.0f)  to (-10.0f,3.0f)*/
      shape.Set(b2Vec2(-20.0f,3.0f) , b2Vec2(-10.0f,3.0f));
      ground->CreateFixture(&shape, 0.0f); /// add a fixture to the existing body with the new shape definition and having 0 density.    
    }

    ///  
    ///<br> 3. a) <b>Dominos at the top</b>
    //Dominos
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.5f, 2.0f);/// create a 1 X 4 rectangle called shape (type b2PolygonShape) 
	
      b2FixtureDef fd;/// define a fixture fd of type b2FixtureDef
      fd.shape = &shape;///set its shape to variable shape defined above
      fd.density = 1.0f;/// set its density to 1
      fd.friction = 0.1f;/// set friction to 0.1
		
      /** create 5 identical dominoes in the b2World separated by 3 unit along horizontal axis */ 
      for (int i = 0; i < 5; ++i)
	{
	  b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	  bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	  
	  /** bodies are at 39.5f along vertical axis and start from -37.5f onwards in horizontal axis*/
	  bd.position.Set(-37.5f + 3.0f * i, 39.5f);
	  b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  body->CreateFixture(&fd);/// add the fixture fd to the body
	}
    }
    // ball/sphere after the dominos on top shelf
    ///<br> 3. b) <b>ball/sphere after the dominos on top shelf</b>    
    {
    	/// spherebody is a b2Body* variable which refers to the sphere
    	b2Body* spherebody;
    	/// create a circular shape of radius 1.0 called circle (b2CircleShape type)
	b2CircleShape circle;
	circle.m_radius = 1.0;
	b2FixtureDef ballfd;/// define a fixture ballfd
      ballfd.shape = &circle;/// set its shape to circle defined above
      ballfd.density = 2.0f;///  set its density to 2
      ballfd.friction = 0.1f;/// set friction to 0.1
      ballfd.restitution = 0.0f;/// set restitution to 0 since we don't need ball to bounce.
      b2BodyDef ballbd;/// b2BodyDef type variable ballbd which describes the body.
	ballbd.type = b2_dynamicBody;/// body is dynamic i.e it can move
	ballbd.position.Set(-20.5f,38.5f );/// the ball is at (-20.5f,38.5f )
	spherebody = m_world->CreateBody(&ballbd);///create the body using ballbd
	spherebody->CreateFixture(&ballfd);/// add fixture ballfd defined above to the body
      
    }  

    ///  	
    /// <br>4. <b>The pendulum that knocks down the dominos</b>
    //The pendulum that knocks the dominos off
    {
    	/** It Consists of two bodies and a joint.*/
      b2Body* b2;/// <b>b2 (the first body , which is just a dummy body used to create the joint )</b> is b2Body* variable which represents a static body.
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.0f, 0.0f);/// create a 0X0 rectangle called shape (type b2PolygonShape).
	  
	b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	bd.position.Set(-38.0f, 44.5f);/// set position of first body to (-38.0f, 44.5f)
	b2 = m_world->CreateBody(&bd);/// create body using bd
	b2->CreateFixture(&shape,0);/// add a fixture to the body with shape as defined above amd density 0
      }
	
      b2Body* b4;///<b> b4 (the second body , which is the pendulum bob)</b> is b2Body* variable which represents a dynamic body.
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 0.25f);/// create a 0.5 X 0.5 rectangle called shape (type b2PolygonShape)
	  
	b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	bd.type = b2_dynamicBody;/// it is dynamic i.e it can move
	bd.position.Set(-41.0f, 41.5f);/// set position of second body to (-41.0f, 41.5f)
	b4 = m_world->CreateBody(&bd);/// create body using bd
	b4->CreateFixture(&shape, 20.0f);/// add a fixture to the body with shape as defined above and having density 20
      }
      	
	/** <b>create a revolute joint to make the pendulum pivot</b>*/
      b2RevoluteJointDef jd;/// jd is a revolute (revolving) joint definition of type b2RevoluteJointDef
      b2Vec2 anchor;/// create a 2d array called anchor to store location of anchor of point
      anchor.Set(-38.0f, 44.5f);///  set coordinates of anchor to (-38.0f, 44.5f)
      jd.Initialize(b2, b4, anchor);/// initialise jd : define the 2 bodies (b2 and b4) and location of common anchor
      m_world->CreateJoint(&jd);/// create the joint using jd
    }
    
    ///  
    /// <br>5. <b>The pulley system</b>   
    /**It consists of 2 bodies and a pulley joint.*/
    //The pulley system
    {
      b2BodyDef *bd = new b2BodyDef;/// b2BodyDef* type variable bd which describes the 2 bodies
      bd->type = b2_dynamicBody;/// Both the bodies are dynamic(i.e they can move)
      bd->position.Set(-12.0f,35.0f);/// first body is set at position (-12.0f,35.0f)
      bd->fixedRotation = true;/// bodies not allowed to rotate
      
      ///
      ///<br> 5. a)<b> The open box (first body of the pulley)</b>
      //The open box
      b2FixtureDef *fd1 = new b2FixtureDef;/// define a fixture fd1 ( type  b2FixtureDef*)
      fd1->density = 20.0;/// set density to 20
      fd1->friction = 0.5;/// set friction to 0.5
      fd1->restitution = 0.f;/// set restitution to 0
      fd1->shape = new b2PolygonShape;
      b2PolygonShape bs1;
      bs1.SetAsBox(2.0,0.1, b2Vec2(1.5f,-3.f), 0);/// create a shape bs1 ( type b2PolygonShape) which is a 4 X 0.2 rectangle centred at (1.5,-3) and rotated by 0 rad 
      fd1->shape = &bs1;/// set fd1's shape member to bs1
      b2FixtureDef *fd2 = new b2FixtureDef;/// define another fixture fd2 ( type  b2FixtureDef*)
      fd2->density = 20.0;/// set density to 20
      fd2->friction = 0.5;/// set friction to 0.5
      fd2->restitution = 0.f;/// set restitution to 0
      fd2->shape = new b2PolygonShape;
      b2PolygonShape bs2;
      bs2.SetAsBox(0.1,1.1, b2Vec2(3.5f,-2.0f), 0);/// create a shape bs ( type b2PolygonShape) which is a 0.2 X 2.2 rectangle centred at (3.5,-2.0) and rotated by 0 rad 
      fd2->shape = &bs2;/// set fd2's shape member to bs2
      b2FixtureDef *fd3 = new b2FixtureDef;/// another fixture definition fd3 ( type  b2FixtureDef*)
      fd3->density = 20.0;/// set density to 20
      fd3->friction = 0.5;/// set friction to 0.5
      fd3->restitution = 0.f;/// set restitution to 0
      fd3->shape = new b2PolygonShape;
      b2PolygonShape bs3;
      bs3.SetAsBox(0.1,1.1, b2Vec2(-0.5f,-2.0f), 0);/// bs3 is a 0.2 X 2.2 rectangle centred at (-0.5,-2.0) and rotated by 0 rad 
      fd3->shape = &bs3;/// set fd3's shape member to bs3
       
      b2Body* box1 = m_world->CreateBody(bd);/// create the first body box1 (of type b2Body*) using bd which is an open box.
      box1->CreateFixture(fd1);/// add fixture fd1 to box1
      box1->CreateFixture(fd2);/// add fixture fd2 to box1
      box1->CreateFixture(fd3);/// add fixture fd3 to box1 (coordinates of the fixtures are wrt box1)
	
	///
      ///<br> 5. b)<b> The bar</b>
      //The bar
      bd->position.Set(-1.5,35);/// set position to (-1.5,35) for the second body which is a bar	
      fd1->density = 42.1;/// set density of fd1 (fixture created earlier) to 42.1. this ensures that the 2 bodies are of nearly equal weight.	  
      b2Body* box2 = m_world->CreateBody(bd);///create the second body box2 (of type b2Body*) using bd 
      box2->CreateFixture(fd1);/// add fixture fd1 to box2
	
	///<br>
      /// <b>The pulley joint</b>
      //The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();/// define a b2PulleyJointDef* type variable called myjoint to create pulley joint / pivot
      
      b2Vec2 worldAnchorGround1(-10.5, 42); /// Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(0, 42); /// Anchor point for ground 2 in world axis
      /** ratio is quantity such that (length1 + ratio* length2 = constant) where length1 and length2 are distances from bodies to their respective ground anchors .*/
      float32 ratio = 1.0f;/// set ratio to 1
      /** initialise myjoint so that
      <ul>
      <li> box1 and box2 are the 2 bodies</li>
      <li> worldAnchorGround1 (-10.5, 42) and worldAnchorGround2(0, 42) are the ground anchors respectively</li>
      <li> box1->GetWorldCenter() and box2->GetWorldCenter() are the local anchors respectively.</li></ul> also pass ratio as an argument while initialising myjoint
      */
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);/// create joint in b2World using myjoint
    }

	///
    ///<br> 6. <b>The see-saw system near the top</b>
    //The see-saw system near the top
    {
      /// 6. a)<b>The triangle wedge</b>
      //The triangle wedge
      b2Body* sbody;/// sbody is a b2Body* variable which represents the wedge
      b2PolygonShape poly;/// poly is a polygon shape we will create of type b2Polygonshape 
      b2Vec2 vertices[3];/// create an 2D array using b2Vec2 of size 3 which will store coordinates of the vertices of the polygon poly
      /** store the positions of the 3 vertices. (-2,0) , (2,0 ) and (0,3)*/
      vertices[0].Set(-2,0);
      vertices[1].Set(2,0);
      vertices[2].Set(0,3);
      poly.Set(vertices, 3);///create a triangular shape called poly with the 3 vertices as specified above
      b2FixtureDef wedgefd;/// define a b2FixtureDef variable wedgefd to create a fixture for sbody
      wedgefd.shape = &poly;///set its shape to poly defined above
      wedgefd.density = 2.0f;/// set its density to 2
      wedgefd.friction = 0.0f;/// set its friction to 0 
      wedgefd.restitution = 0.0f;/// set its restitution to 0
      b2BodyDef wedgebd;/// define a b2BodyDef variable wedgebd which will be used to create the actual wedge
      wedgebd.position.Set(12.0f, 34.0f);/// let its position be (12,34) in world coordinates i.e it is on the ground
      sbody = m_world->CreateBody(&wedgebd);/// create the body using wedgebd
      sbody->CreateFixture(&wedgefd);/// add the fixture wedgefd to sbody  ( the vertices of wedgefd will have coordinates wrt sbody )
	
	///
      ///<br> 6. b)<b>The plank on top of the wedge</b>
      //The plank on top of the wedge
      b2PolygonShape shape;/// shape is a polygon shape we will create of type  b2PolygonShape
      shape.SetAsBox(11.0f, 0.1f);/// it is a 22 X 0.2 rectangle 
      b2BodyDef bd2;/// define a b2BodyDef variable bd2 which will be used to create the plank
      bd2.position.Set(12.0f, 37.0f);/// let its position be (12,37) in world coordinates i.e it touches the upper vertex of wedge.
      bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
      b2Body* body = m_world->CreateBody(&bd2);/// create body ( type  b2Body) using bd2
      b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable fd2 to create a fixture for body
      fd2->density = 0.5f;/// set its density to 0.5
      fd2->shape = new b2PolygonShape;
      fd2->shape = &shape;/// set its shape to the shape variable defined above
      body->CreateFixture(fd2); /// add the fixture fd2 to body
	
	/// 
      /**<br> 6. c) <b>create a revolute joint at the point where the wedge and the plank meet for the see-saw.</b>*/
      b2RevoluteJointDef jd;///define a revolute (revolving) joint jd of type b2RevoluteJointDef
      b2Vec2 anchor;/// create a 2d array using b2vec2 called anchor to store location of anchor of point
      anchor.Set(12.0f, 37.0f);/// set coordinates of anchor to (12,37)
      jd.Initialize(sbody, body, anchor);/// initialise jd : define the 2 bodies (sbody and body) and location of common anchor
      jd.enableLimit = true;/// limit the rotation of the joint
  	jd.lowerAngle = -0.707;
  	jd.upperAngle =  0.707;/// joint can rotate from +0.707 to -0.707 radians only
      m_world->CreateJoint(&jd);/// create the joint using jd
	
	///<br> 6. d) <b>ball on left side of see saw</b>
	b2Body* spherebody1;///spherebody1 is a b2Body* variable which refers to a sphere
	b2CircleShape circle1;/// create a circular shape of radius 1.0 called circle1 (b2CircleShape type)
	circle1.m_radius = 1.0;
	b2FixtureDef ballfd1;/// define a fixture ballfd1
      ballfd1.shape = &circle1;/// set its shape to circle1 defined above
      ballfd1.density = 0.4f;///  set its density to 0.4
      ballfd1.friction = 0.1f;/// set its friction to 0.1
      ballfd1.restitution = 0.0f;/// set its restitution to 0
      b2BodyDef ballbd1;/// b2BodyDef type variable ballbd1 which describes the body.
	ballbd1.type = b2_dynamicBody;/// body is dynamic i.e it can move
	ballbd1.position.Set(5.0f,38.0f );/// set position of ball (5,38)
	spherebody1 = m_world->CreateBody(&ballbd1);///create the body using ballbd1
	spherebody1->CreateFixture(&ballfd1);/// add fixture ballfd1 defined above to the body
	
	/// <br> 6. e) <b>ball on right side of see saw.</b>
	ballbd1.position.Set(19.0f,38.0f);///set position of ballbd1 (already defined above) to (19,38)
	spherebody1 = m_world->CreateBody(&ballbd1);///create the second ball using new definition of ballbd1
	spherebody1->CreateFixture(&ballfd1);/// add fixture ballfd1 defined above to the body
	
	/// <br> 6. f) <b> vertical obstacle which prevents ball on left from falling down to the right.</b>
	
	b2PolygonShape shape2;
      shape2.SetAsBox(2.0f, 0.01f, b2Vec2(0.f,0.f), 1.57f);///shape2 is a 4 X 0.02 rectangle centred at (0,0) wrt the body it will be attached to, and it is rotated by (pi/2) radians in anti-clockwise direction.
	
      b2BodyDef bd4;/// b2BodyDef type variable bd4 which describes the body.
      bd4.position.Set(12.0f, 39.2f);/// set position of body (12, 39.2) so that it is exactly at the centre of the see saw system.
      b2Body* ground2 = m_world->CreateBody(&bd4);///ground2 is a b2Body* variable which refers to the obstacle.
      ground2->CreateFixture(&shape2, 0.0f);/// add a fixture to the body with shape2 as defined above and having 0 density
      
    } 
    
    ///<br>
    {
	b2Body* spherebody;///spherebody is a b2Body* variable which refers to a sphere
	b2CircleShape circle;
	circle.m_radius = 1.0;/// create a circular shape of radius 0.5 called circle (b2CircleShape type)
	b2FixtureDef ballfd;/// define a fixture ballfd
      ballfd.shape = &circle;/// set its shape to circle defined above
      ballfd.density = 0.2f;///  set its density to 0.2
      ballfd.friction = 0.01f;/// set friction to 0.01 so that balls can slide easily
      ballfd.restitution = 0.0f;/// set restitution to 0

	///7. <b>balls on platform above the pulley system near the bottom</b>
	for (int i = 0; i < 2; ++i){
    		b2BodyDef ballbd;/// b2BodyDef type variable ballbd which describes the body.
		ballbd.type = b2_dynamicBody;/// body is dynamic i.e it can move
		/** bodies are at 12.0f along vertical axis and start from -9.0f onwards in horizontal axis at a distance of 3 from each other
		towards the negative x- axis. */
		ballbd.position.Set(-9.0f - 3.0f * i,12.0f );
		spherebody = m_world->CreateBody(&ballbd);///create the body using ballbd
		spherebody->CreateFixture(&ballfd);/// add fixture ballfd defined above to the body
	}	
    }
    
    ///
    ///<br>8. <b> pulley system near the bottom</b>
    {
   	b2BodyDef *bd = new b2BodyDef;/// b2BodyDef* type variable bd which describes the 2 bodies
      bd->type = b2_dynamicBody;/// Both the bodies are dynamic(i.e they can move)
      bd->position.Set(-16.0f,6.0f);/// first body is set at position (-16,6)
      bd->fixedRotation = true;/// bodies not allowed to rotate
      
      ///
      ///<br> 8. a)<b> The open box</b>
      //The open box
      b2FixtureDef *fd1 = new b2FixtureDef;/// define a fixture fd1 ( type  b2FixtureDef*)
      fd1->density = 21.5;/// set density to 21.5
      fd1->friction = 0.5;/// set friction to 0.5
      fd1->restitution = 0.f;/// set restitution to 0
      fd1->shape = new b2PolygonShape;
      b2PolygonShape bs1;
      bs1.SetAsBox(4.0,0.1); /// create a shape bs1( type b2PolygonShape) which is a 8 X 0.2. 
      fd1->shape = &bs1;/// set fd1's shape member to bs1
      
      b2FixtureDef *fd2 = new b2FixtureDef;/// define another fixture fd2 ( type  b2FixtureDef*)
      fd2->density = 20.0;/// set density to 20
      fd2->friction = 0.5;/// set friction to 0.5
      fd2->restitution = 0.f;/// set restitution to 0
      fd2->shape = new b2PolygonShape;
      b2PolygonShape bs2;
      bs2.SetAsBox(0.1,1.0, b2Vec2(4.0f,1.0f), 0);/// create a shape bs ( type b2PolygonShape) which is a 0.2 X 2 rectangle centred at (4,1) wrt body it will be attached to, and rotated by 0 radians 
      fd2->shape = &bs2;/// set fd2's shape member to bs2
      b2FixtureDef *fd3 = new b2FixtureDef;/// another fixture definition fd3 ( type  b2FixtureDef*)
      fd3->density = 20.0;/// set density to 20
      fd3->friction = 0.5;/// set friction to 0.5
      fd3->restitution = 0.f;/// set restitution to 0
      fd3->shape = new b2PolygonShape;
      b2PolygonShape bs3;
      bs3.SetAsBox(0.1,1, b2Vec2(-4.0f,1.0f), 0);/// bs3 is a 0.2 X 2 rectangle centred at (-4,1)wrt body it will be attached to, and rotated by 0 radians 
      fd3->shape = &bs3;/// set fd3's shape member to bs3
       
      b2Body* box1 = m_world->CreateBody(bd);/// create the first body box1 (of type b2Body*) using bd which is an open box.
      box1->CreateFixture(fd1);/// add fixture fd1 to box1
      box1->CreateFixture(fd2);/// add fixture fd2 to box1
      box1->CreateFixture(fd3);/// add fixture fd3 to box1 (coordinates of the fixtures are wrt box1)
	
	///
      ///<br> 8. b)<b> The domino</b>
      //The domino
      bd->position.Set(-8.5f,1.5f);///set position of the body to (-8.5,1.5).
      b2PolygonShape bs4;
      bs4.SetAsBox(0.2 , 1.5 , b2Vec2( 0.0f ,0.f) , 0); /// bs4 is a 0.4 X 3 rectangle centred at (0,0) wrt the body it will be attached to, and it is rotated by 0 radians.
      fd1->shape = &bs4;/// set fd1's shape member to bs4.
      fd1->density = 42.1;/// set density of fd1 (fixture created earlier) to 42.1. This ensures two sides of pulley are balanced.	  
      b2Body* box2 = m_world->CreateBody(bd);///create the second body box2 (of type b2Body*) using bd 
      box2->CreateFixture(fd1);/// add fixture fd1 to box2
	
	///<br>
      /// <b>The pulley joint</b>
      //The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();/// define a b2PulleyJointDef* type variable called myjoint to create pulley joint / pivot
      
      b2Vec2 worldAnchorGround1(-16, 9.5); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(-8.5, 9.5); // Anchor point for ground 2 in world axis
      float32 ratio = 1.0f;/// set ratio to 1
      /** initialise myjoint so that
      <ul>
      <li> box1 and box2 are the 2 bodies</li>
      <li> worldAnchorGround1 (-16, 9.5) and worldAnchorGround2(-8.5, 9.5) are the ground anchors respectively</li>
      <li> box1->GetWorldCenter() and box2->GetWorldCenter() are the local anchors respectively.</li></ul> also pass ratio as an argument while initialising myjoint
      */
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);/// create joint in b2World using myjoint
    }
    
    //
    ///<br>9. <b>dominos near the bottom (which will topple on both sides) </b>
    {
   	b2PolygonShape shape;
      shape.SetAsBox(0.5f, 2.5f);/// create a 1 X 5 rectangle called shape (type b2PolygonShape) 
	
      b2FixtureDef fd;/// define a fixture fd of type b2FixtureDef
      fd.shape = &shape;///set its shape to variable shape defined above
      fd.density = 0.3f;/// set its density to 0.3
      fd.friction = 0.1f;/// set friction to 0.1
		
      /** create 5 identical dominoes in the b2World separated by 1.25 unit along horizontal axis */ 
      for (int i = 0; i < 5; ++i)
	{
	  b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	  bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	  /** bodies are at 8.5f along vertical axis and start from -5.5f onwards in horizontal axis*/
	  bd.position.Set(-5.5f + 1.25f * i, 8.5f);
	  b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  body->CreateFixture(&fd);/// add a fixture fd to the body
	}
	
	///<br> create 2 dominoes on top of the 5 existing dominos
	b2PolygonShape shape1;
      shape1.SetAsBox(0.5f,2.0f);/// create a 1 X 4 rectangle called shape1 (type b2PolygonShape) 
      fd.shape = &shape1;///set shape variable of fd (defined already) to shape1.
	/** create 2 identical dominoes in the b2World separated by 2.8 unit along horizontal axis */ 
	for ( int i =0 ; i<2 ; i++)
	{
		b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	 	bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	 	/** bodies are at 13.0f along vertical axis and start from -4.5f onwards in horizontal axis*/
	 	bd.position.Set( -4.5f + 2.8 * i , 13.0f);
	 	b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  	body->CreateFixture(&fd);/// add a fixture fd to the body  
	}	
    }
    
    
    ///
    ///<br> 10. <b>The revolving horizontal platform</b>
    {
      b2PolygonShape shape, shape2;  
      shape.SetAsBox(2.2f, 0.2f);/// define a 4.4 X 0.4 rectangular shape called shape (b2PolygonShape type)
	  shape2.SetAsBox(2.2f, 0.2f);/// define another 4.4 X 0.4 rectangular shape called shape2 (b2PolygonShape type)
	
      b2BodyDef bd, bd3; /// define b2BodyDef variables bd, bd3 which will be used to create the platform . bd3 is a horizontal bar which falls on the platform and sets it in rotation.
      bd.position.Set(25.0f, 28.0f);/// set position of revolving platform as (25,28)
		bd3.position.Set(28.0f, 30.2f);///set position if bar as (28,30.2)
      bd.type = b2_dynamicBody;  
 		bd3.type = b2_dynamicBody;/// let both the bodies be dynamic i.e they can move
      b2Body* body = m_world->CreateBody(&bd);/// create a b2Body* variable body which Refers to the platform.
      	b2Body* body3 = m_world->CreateBody(&bd3);/// create a b2Body* variable body3 which Refers to the bar.
      b2FixtureDef *fd = new b2FixtureDef; /// define a fixture fd 
      	b2FixtureDef *fd2 = new b2FixtureDef; /// define another fixture fd2 
      fd->density = 10.0f;/// set density of platform as 10
      	fd2->density = 40.0f;///set density of bar as 40
      fd->shape = new b2PolygonShape;
		fd2->shape = new b2PolygonShape;
      fd->shape = &shape; /// set shape member of fd to the shape variable defined above 
		fd2->shape = &shape2;/// set shape member of fd2 to the shape2 variable defined above 
      body->CreateFixture(fd);/// add fixture fd to body
		body3->CreateFixture(fd2);/// add fixture fd2 to body3
		
      /** define a b2BodyDef variable bd2 which will be used to create another body for the pivot/hinge of the platform*/
      b2BodyDef bd2;
      bd2.position.Set(25.0f, 28.0f);/// set its position as (25,28)
      b2Body* body2 = m_world->CreateBody(&bd2);/// body2 is of b2Body* type. Refers to a dummy body created for the hinge.

      b2RevoluteJointDef jointDef;/// create a  revolute (revolving) joint definition called jointDef
      jointDef.bodyA = body;/// its first body is body
      jointDef.bodyB = body2;/// its second body is body2
      jointDef.localAnchorA.Set(0,0);/// set the anchor for body as (0,0) wrt itself
      jointDef.localAnchorB.Set(0,0);/// set the anchor for body2 as (0,0) wrt itself
      jointDef.collideConnected = false;/// disallow collision 
      m_world->CreateJoint(&jointDef);/// create the joint using jointDef
    }

    ///
    /// <br> 11. <b>Sequence of 5 see-saw systems (see saw + ball + base) in lower right portion</b> 
    
    for(int a = 0; a <= 4; a++)
    {
    
	///<br>creates the see saw at y = 8 , and along x axis ,start from 4 and are placed at a distance of 6.
    see_saw_create( 4 + a*6, 8 , 2.85 , 0.1);
    /// creates the ball at end of see saw.
  	b2Body* spherebody;/// spherebody is a b2Body* variable which refers to a sphere
	b2CircleShape circle;
	circle.m_radius = 0.7;/// create a circular shape of radius 1.4 called circle (b2CircleShape type)
	b2FixtureDef ballfd;/// define a fixture ballfd
    ballfd.shape = &circle;/// set its shape to circle defined above
    ballfd.density = 2.0f;///set density to 2
    ballfd.friction = 0.1f;/// set friction to 0.1
    ballfd.restitution = 0.0f;/// set restitution to 0
    b2BodyDef ballbd;/// b2BodyDef type variable ballbd which describes the body.
	ballbd.type = b2_dynamicBody;/// body is dynamic i.e it can move
	ballbd.position.Set(7 + a*6,14);/// set position of the ball as (7 + a*6,14)
	spherebody = m_world->CreateBody(&ballbd);///create the body using ballbd
	spherebody->CreateFixture(&ballfd);/// add fixture ballfd defined above to the body
    
    ///<br> creates a base for the ball at edge of see saw
    b2PolygonShape shape, shape2;  
    shape.SetAsBox(0.3f, 0.3f);///<br> define a 0.6 X 0.6 rectangular shape called shape (b2PolygonShape type)
	
    b2BodyDef bd; /// define a b2BodyDef variable bd which will be used to create the platform 
    bd.position.Set(7 + a*6, 12.0f);/// set its position as (7 + a*6, 12.0f)
    bd.type = b2_dynamicBody;  /// let it be dynamic i.e it can move
    b2Body* body = m_world->CreateBody(&bd);/// create a b2Body* variable body which Refers to the base.
    b2FixtureDef *fd = new b2FixtureDef; /// define a fixture fd 
    fd->density = 10.0f;/// set its density as 10
    fd->shape = new b2PolygonShape;
    fd->shape = &shape; /// set shape member of fd to the shape variable defined above 
    body->CreateFixture(fd);/// add fixture fd to body

    b2BodyDef bd2;
    bd2.position.Set(7 + a*6, 12.0f);/// set its position as (7 + a*6, 12.0f)
    b2Body* body2 = m_world->CreateBody(&bd2);/// body2 is of b2Body* type. Refers to a dummy body created for the hinge.

    b2RevoluteJointDef jointDef;///<br> create a  revolute (revolving) joint definition called jointDef
    jointDef.bodyA = body;/// its first body is body
    jointDef.bodyB = body2;/// its second body is body2
    jointDef.localAnchorA.Set(0,0);/// set the anchor for body as (0,0) wrt itself
    jointDef.localAnchorB.Set(0,0);/// set the anchor for body2 as (0,0) wrt itself
    jointDef.collideConnected = false;/// disallow collision 
    m_world->CreateJoint(&jointDef);/// create the joint using jointDef

 	}   

	///
	/// <br> 12. <b>Sliding Gate</b>
	{	
		/// 12. a)<b>Framing the Gate</b> 
	    b2EdgeShape shape;
    	shape.Set(b2Vec2(-41.0f,33.5f) , b2Vec2(-25.0f,33.5f));/// create <i>shape</i> which is an edge from (-41,33.5) to (-25,33.5)  
		/** This is the upper rod of the sliding-gate*/	

    	b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
    	b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
    	ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
	
		shape.Set(b2Vec2(-41.0f, 23.5f) , b2Vec2(-25.0f, 23.5f));/// create <i>shape</i> which is an edge from (-41,23.5) to (-25,23.5)
		/** This is the lower rod of the sliding-gate*/
    	ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density

		shape.Set(b2Vec2(-41.0f, 23.5f) , b2Vec2(-41.0f, 33.5f));/// create <i>shape</i> which is an edge from (-41,23.5) to (-41,33.5)
		/** This is the left-end of the sliding-gate. The gate will slide upto this point*/
		ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density

		///
		/// <br> 12. b)<b>The Gate Connection</b>
    	b2PolygonShape shape2, shape3;
    	shape2.SetAsBox(2.5, 5);/// create a 5X10 rectangle called <i>shape2</i> (type b2PolygonShape) 
		/** This is the gate between the rods*/
		shape3.SetAsBox(2.5, 2.5);/// create a 5X5 rectangle called <i>shape3</i> (type b2PolygonShape)
		/** This is the lower box which must be moved to slide the gate*/
		
    	b2BodyDef bd2;/// define a b2BodyDef variable <i>bd2</i> which will be used to create the gate/box
    	bd2.position.Set(-27.5 , 28.5);/// let its position be (-27.5,28.5) in world coordinates
    	bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
    	b2Body* body = m_world->CreateBody(&bd2);/// create <i>body</i> ( type  b2Body) using <i>bd2</i>
    	b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable <i>fd2</i> to create a fixture for <i>body</i>
    	fd2->density = 0.00001f;/// set its density to 0.00001; this is needed so as to allow sliding when pulled under constraint
    	fd2->shape = new b2PolygonShape;
    	fd2->shape = &shape2;/// set its shape to the <i>shape2</i> variable defined above
    	body->CreateFixture(fd2); /// add the fixture <i>fd2</i> to <i>body</i>

		bd2.position.Set( -27.5 , 2.5);/// let the position of this body be (-27.5, 2.5) in world coordinates
		b2Body* body2 = m_world->CreateBody(&bd2);/// create <i>body2</i> (type b2Body) using <i>bd2</i>
		fd2->density = 0.001f;/// set its density to 0.001; this is needed to allow effective sliding of gate by the box, also leading to  easy movement of box after the ball hits it
		fd2->shape = &shape3;/// set its shape to the <i>shape3</i> variable defined above
		body2->CreateFixture(fd2);/// add the fixture <i>fd2></i> to <i>body2</i>
	    
		b2DistanceJointDef* dist = new b2DistanceJointDef();/// defined a new distance joint variable <i>dist</i>(type b2DistanceJointDef*)
		dist->Initialize(body, body2, body->GetWorldCenter(), body2->GetWorldCenter());/// set the joint between <i>body</i> and <i>body2</i>
		/** This sets a minimum-distance constraint between the box and the gate, thus leading to sliding of gate on movement of box*/	
		m_world->CreateJoint(dist);/// create the joint defined above
   
    }
    
    /// 
    /// <br> 13. <b>Balls and Hinged bars at the bottom</b>
	{
	for(int cust = 0; cust < 4; cust++)
	{
		/// 13. a)<b>Bars and the Hinges</b>
  	    b2Body* b2;
  	    {
			b2PolygonShape shape;
			shape.SetAsBox(0.25f, 1.5f);/// create a rectangular shape of size 0.5X3
			/** This serves as the bar*/
	  
			b2BodyDef bd;/// define a b2BodyDef variable <i>bd</i> which will be used to create the bar 
			bd.type = b2_dynamicBody;/// let it be dynamic, i.e., it can move
			bd.position.Set(30 - cust*10, 1.5);/// set its position as (30 - cust*10, 1.5), where <i>cust</i> goes from 0 to 3
			/** The definition in a "for" loop helps create four bars without rewriting code*/
			b2 = m_world->CreateBody(&bd);/// create body <i>b2</i> ( type  b2Body) using <i>bd</i>
			b2->CreateFixture(&shape, 0.01f);/// add a fixture to the body with shape as defined above and having 0.01 density
  	    }
	
  	    b2Body* b4;
  	    {
			b2PolygonShape shape;
			shape.SetAsBox(0.25f, 0.25f);/// create a rectangular shape of size 0.5x0.5
			/** This serves as the dummy-body for the hinge*/
			 	  
			b2BodyDef bd;/// define a b2BodyDef variable <i>bd</i> which will be used to create the dummy-hinge body
			/** Note that it is static, since we do not want the dummy body to move*/
			bd.position.Set(30 - cust*10, 3);/// set its position as (30 - cust*10, 3), where <i>cust</i> goes from 0 to 3
			/** The definition in a "for" loop helps create four bars without rewriting code*/
			b4 = m_world->CreateBody(&bd);/// create body <i>b4</i> ( type  b2Body) using <i>bd</i>
			b4->CreateFixture(&shape, 0.01f);/// add a fixture to the body with shape as defined above and having 0.01 density
  	    }
	
  		b2RevoluteJointDef jointDef;///<br> create a  revolute (revolving) joint definition called jointDef
  		jointDef.bodyA = b2;/// its first body is b2
    	jointDef.bodyB = b4;/// its second body is b4
	    jointDef.localAnchorA.Set(0,1.5);/// set the anchor for <i>b2</i> as (0,1.5) wrt itself
	    /** Thus the bar gets hinged at its top*/
    	jointDef.localAnchorB.Set(0,0);/// set the anchor for <i>b4</i> as (0,0) wrt itself
    	jointDef.collideConnected = false;/// disallow collision 
    	m_world->CreateJoint(&jointDef);/// create the joint using jointDef

		/// 
		///<br> 13. b)<b>The train of balls</b>
		b2Body* spherebody;
			
		b2CircleShape circle;/// create a circle as the shape
		circle.m_radius = 1.0;/// let the radius of the circle be 1
		b2FixtureDef ballfd;/// define a fixture ballfd
        ballfd.shape = &circle;/// set its shape to circle defined above
	    ballfd.density = 0.1f;///  set its density to 0.1
	    ballfd.friction = 0.001f;/// set friction to 0.001
	    ballfd.restitution = 0.0f;/// set restitution to 0 since we don't need ball to bounce.
	    b2BodyDef ballbd;/// b2BodyDef type variable ballbd which describes the body.
		ballbd.type = b2_dynamicBody;/// body is dynamic i.e it can move

		ballbd.position.Set(28 - 10*cust,1.0 );/// the ball is at (28-10*cust, 1)
		spherebody = m_world->CreateBody(&ballbd);///create the body using ballbd
		spherebody->CreateFixture(&ballfd);/// add fixture ballfd defined above to the body
 
	}
}   
   
}

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
