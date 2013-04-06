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
   wedgebd.position.Set(x_pos, y_pos);/// let its position be (30,0) in world coordinates i.e it is on the ground
   sbody = m_world->CreateBody(&wedgebd);/// create the body using wedgebd
   sbody->CreateFixture(&wedgefd);/// add the fixture wedgefd to sbody  ( the vertices of wedgefd will have coordinates wrt sbody )

	///<br>
    /// 10. b)<b>The plank on top of the wedge</b>
    //The plank on top of the wedge
    b2PolygonShape shape;/// shape is a polygon shape we will create of type  b2PolygonShape
    shape.SetAsBox(plank_length, plank_breadth);/// it is a 15 X 0.2 rectangle 
    b2BodyDef bd2;/// define a b2BodyDef variable bd2 which will be used to create the plank
    bd2.position.Set(x_pos, y_pos + 3);/// let its position be (30,1.5) in world coordinates i.e it touches the upper vertex of wedge.
    bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
    b2Body* body = m_world->CreateBody(&bd2);/// create body ( type  b2Body) using bd2
    b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable fd2 to create a fixture for body
    fd2->density = 0.5f;/// set its density to 1
    fd2->shape = new b2PolygonShape;
    fd2->shape = &shape;/// set its shape to the shape variable defined above
    body->CreateFixture(fd2); /// add the fixture fd2 to body
	
	///<br> 
    /** 10. c) <b>create a revolute joint at the point where the wedge and the plank meet for the see-saw.</b>*/
    b2RevoluteJointDef jd;///define a revolute (revolving) joint jd of type b2RevoluteJointDef
    b2Vec2 anchor;/// create a 2d array using b2vec2 called anchor to store location of anchor of point
    anchor.Set(x_pos, y_pos+3);/// set coordinates of anchor to (30,1.5)
    jd.Initialize(sbody, body, anchor);/// initialise jd : define the 2 bodies (sbody and body) and location of common anchor
    jd.enableLimit = true;/// limit the rotation of the joint
  	jd.lowerAngle = -1;
  	jd.upperAngle =  1;
    m_world->CreateJoint(&jd);/// create the joint using jd


}



  dominos_t::dominos_t()
  {
    /// <br> 
    /// 1. <b>Ground</b>
    //Ground
    b2Body* b1;/// a rectangular shaped body. b1 is a b2Body* variable which represents the body.
    {
      /**shape is a line segment (b2Edgeshape) of length 92 with end points (-46,0) and (46,0)*/
      b2EdgeShape shape;
      shape.Set(b2Vec2(-46.0f, 0.0f), b2Vec2(46.0f, 0.0f));
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
      b1 = m_world->CreateBody(&bd);/// create the body b1 using bd
      b1->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
    }
      
     /// <br> 
    /// 2. <b>Top horizontal shelf</b>
    //Top horizontal shelf
    {
    	/**shape is a line segment (b2Edgeshape) of length 13 with end points (-43.0f,37.5f) and (-30.0f,37.5f)*/
      b2EdgeShape shape;
      shape.Set(b2Vec2(-41.0f,37.5f) , b2Vec2(-15.0f,37.5f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
    }

	/// <br>
	/// <b> A horizontal shelf just below the open box (connected to the pulley)
    {
	  b2EdgeShape shape;
      shape.Set(b2Vec2(-13.0f,26.0f) , b2Vec2(-7.0f,26.0f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density	
    }
    
    //Tilted shelf connected to the topmost horizontal shelf
    {
      b2PolygonShape shape;
      shape.SetAsBox(2.0f, 0.01f, b2Vec2(1.414f,-1.414f), -0.707f);
	
      b2BodyDef bd;
      bd.position.Set(-15.0f, 37.5f);
      b2Body* ground = m_world->CreateBody(&bd);
      ground->CreateFixture(&shape, 0.0f);
    }
    
    //other platforms
    {
      // horizontal shelf below the see saw
    	b2EdgeShape shape;
      shape.Set(b2Vec2(4.0f,23.0f) , b2Vec2(30.0f,23.0f));  
	
      b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
      b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
      
      // tilted platform 
      shape.Set(b2Vec2(4.0f,23.0f) , b2Vec2(0.0f,18.0f));
      ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density
      
      shape.Set(b2Vec2(-7.0f,6.0f) , b2Vec2(38.0f,6.0f));
      ground->CreateFixture(&shape, 0.0f);
      
      shape.Set(b2Vec2(-7.0f,6.0f) , b2Vec2(-7.0f,11.0f));
      ground->CreateFixture(&shape, 0.0f);
      
      shape.Set(b2Vec2(-7.0f,11.0f) , b2Vec2(-15.0f,11.0f));
      ground->CreateFixture(&shape, 0.0f);
      
      shape.Set(b2Vec2(40.0f,0.0f) , b2Vec2(46.0f,8.0f));
      ground->CreateFixture(&shape, 0.0f); 
      
      shape.Set(b2Vec2(-20.0f,3.0f) , b2Vec2(-10.0f,3.0f));
      ground->CreateFixture(&shape, 0.0f);     
    }

    /// <br> 
    /// 3. <b>Dominos</b>
    //Dominos
    {
      b2PolygonShape shape;
      shape.SetAsBox(0.5f, 2.0f);/// create a 0.2 X 2 rectangle called shape (type b2PolygonShape) 
	
      b2FixtureDef fd;/// define a fixture fd of type b2FixtureDef
      fd.shape = &shape;///set its shape to variable shape defined above
      fd.density = 1.0f;/// set its density to 20
      fd.friction = 0.1f;/// set friction to 0.1
		
      /** create 10 identical dominoes in the b2World separated by 3 unit along horizontal axis */ 
      for (int i = 0; i < 5; ++i)
	{
	  b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	  bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	  
	  /** bodies are at 31.25f along vertical axis and start from -35.5f onwards in horizontal axis*/
	  bd.position.Set(-37.5f + 3.0f * i, 39.5f);
	  b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  body->CreateFixture(&fd);/// add a fixture fd to the body
	}
    }
    // ball/sphere after the dominos on top shelf
    {
    	b2Body* spherebody;
	b2CircleShape circle;
	circle.m_radius = 1.0;
	b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 2.0f;
      ballfd.friction = 0.1f;
      ballfd.restitution = 0.0f;
      b2BodyDef ballbd;
	ballbd.type = b2_dynamicBody;
	ballbd.position.Set(-20.5f,38.5f );
	spherebody = m_world->CreateBody(&ballbd);
	spherebody->CreateFixture(&ballfd);
      
    }  

    /// <br> 	
    /// 5. <b>The pendulum that knocks the dominos off</b>
    //The pendulum that knocks the dominos off
    {
    	/** It Consists of two bodies and a joint.*/
      b2Body* b2;/// <b>b2 (the first body)</b> is b2Body* variable which represents a static body.
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.0f, 0.0f);/// create a 0X0 rectangle called shape (type b2PolygonShape)
	  
	b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	bd.position.Set(-38.0f, 44.5f);/// set position of first body to (-43,44.5)
	b2 = m_world->CreateBody(&bd);/// create body using bd
	b2->CreateFixture(&shape,0);/// add a fixture to the body with shape as defined above amd density 0
      }
	
      b2Body* b4;///<b> b4 (the second body)</b> is b2Body* variable which represents a dynamic body.
      {
	b2PolygonShape shape;
	shape.SetAsBox(0.25f, 0.25f);/// create a 0.5X0.5 rectangle called shape (type b2PolygonShape)
	  
	b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	bd.type = b2_dynamicBody;/// it is dynamic i.e it can move
	bd.position.Set(-41.0f, 41.5f);/// set position of second body to (-45,40)
	b4 = m_world->CreateBody(&bd);/// create body using bd
	b4->CreateFixture(&shape, 20.0f);/// add a fixture to the body with shape as defined above and having density 2
      }
      	
	/** <b>create a revolute joint to make the pendulum pivot</b>*/
      b2RevoluteJointDef jd;/// jd is a revolute (revolving) joint definition of type b2RevoluteJointDef
      b2Vec2 anchor;/// create a 2d array called anchor to store location of anchor of point
      anchor.Set(-38.0f, 44.5f);///  set coordinates of anchor to (-43,44.5)
      jd.Initialize(b2, b4, anchor);/// initialise jd : define the 2 bodies (b2 and b4) and location of common anchor
      m_world->CreateJoint(&jd);/// create the joint using jd
    }
    
    /// <br> 
    /// 7. <b>The pulley system</b>   
    /**It consists of 2 bodies and a pulley joint.*/
    //The pulley system
    {
      b2BodyDef *bd = new b2BodyDef;/// b2BodyDef* type variable bd which describes the 2 bodies
      bd->type = b2_dynamicBody;/// Both the bodies are dynamic(i.e they can move)
      bd->position.Set(-12.0f,35.0f);/// first body is set at position (-10,15)
      bd->fixedRotation = true;/// bodies not allowed to rotate
      
      ///<br>
      /// 7. <b>a) The open box</b>
      //The open box
      b2FixtureDef *fd1 = new b2FixtureDef;/// define a fixture fd1 ( type  b2FixtureDef*)
      fd1->density = 20.0;/// set density to 10
      fd1->friction = 0.5;/// set friction to 0.5
      fd1->restitution = 0.f;/// set restitution to 0
      fd1->shape = new b2PolygonShape;
      b2PolygonShape bs1;
      bs1.SetAsBox(2.0,0.1, b2Vec2(1.5f,-3.f), 0);/// create a shape bs1 ( type b2PolygonShape) which is a 4 X 0.4 rectangle centred at (0,-1.9) and rotated by 0 rad 
      fd1->shape = &bs1;/// set fd1's shape member to bs1
      b2FixtureDef *fd2 = new b2FixtureDef;/// define another fixture fd2 ( type  b2FixtureDef*)
      fd2->density = 20.0;/// set density to 10
      fd2->friction = 0.5;/// set friction to 0.5
      fd2->restitution = 0.f;/// set restitution to 0
      fd2->shape = new b2PolygonShape;
      b2PolygonShape bs2;
      bs2.SetAsBox(0.1,1.1, b2Vec2(3.5f,-2.0f), 0);/// create a shape bs ( type b2PolygonShape) which is a 0.4 X 4 rectangle centred at (2,0) and rotated by 0 rad 
      fd2->shape = &bs2;/// set fd2's shape member to bs2
      b2FixtureDef *fd3 = new b2FixtureDef;/// another fixture definition fd3 ( type  b2FixtureDef*)
      fd3->density = 20.0;/// set density to 10
      fd3->friction = 0.5;/// set friction to 0.5
      fd3->restitution = 0.f;/// set restitution to 0
      fd3->shape = new b2PolygonShape;
      b2PolygonShape bs3;
      bs3.SetAsBox(0.1,1.1, b2Vec2(-0.5f,-2.0f), 0);/// bs3 is a 0.4 X 4 rectangle centred at (-2,0) and rotated by 0 rad 
      fd3->shape = &bs3;/// set fd3's shape member to bs3
       
      b2Body* box1 = m_world->CreateBody(bd);/// create the first body box1 (of type b2Body*) using bd which is an open box.
      box1->CreateFixture(fd1);/// add fixture fd1 to box1
      box1->CreateFixture(fd2);/// add fixture fd2 to box1
      box1->CreateFixture(fd3);/// add fixture fd3 to box1 (coordinates of the fixtures are wrt box1)
	
	///<br>
      /// 7.<b> b) The bar</b>
      //The bar
      bd->position.Set(-1.5,35);/// set position to (10,15) for the second body which is a bar	
      fd1->density = 42.1;/// set density of fd1 (fixture created earlier) to 34. this ensures second body of pulley is heavier.	  
      b2Body* box2 = m_world->CreateBody(bd);///create the second body box2 (of type b2Body*) using bd 
      box2->CreateFixture(fd1);/// add fixture fd1 to box2
	
	///<br>
      /// <b>The pulley joint</b>
      //The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();/// define a b2PulleyJointDef* type variable called myjoint to create pulley joint / pivot
      
      b2Vec2 worldAnchorGround1(-10.5, 42); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(0, 42); // Anchor point for ground 2 in world axis
      /** ratio is quantity such that (length1 + ratio* length2 = constant) where length1 and length2 are lengths of the ropes on 2 sides of
       pulley.*/
       /**It can cause one side of pulley to slide faster.*/
      float32 ratio = 1.0f;/// set ratio to 1
      /** initialise myjoint so that
      <ul>
      <li> box1 and box2 are the 2 bodies</li>
      <li> worldAnchorGround1 (-10,20) and worldAnchorGround2(10,20) are the ground anchors respectively</li>
      <li> box1->GetWorldCenter() and box2->GetWorldCenter() are the local anchors respectively.</li></ul> also pass ratio as an argument while initialising myjoint
      */
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);/// create joint in b2World using myjoint
    }

	///<br>
    /// 10. <b>The see-saw system at the bottom</b>
    //The see-saw system at the bottom
    {
      /// 10. a)<b>The triangle wedge</b>
      //The triangle wedge
      b2Body* sbody;/// sbody is a b2Body* variable which represents the wedge
      b2PolygonShape poly;/// poly is a polygon shape we will create of type b2Polygonshape 
      b2Vec2 vertices[3];/// create an 2D array using b2Vec2 of size 3 which will store coordinates of the vertices of the polygon poly
      /** store the positions of the 3 vertices. (-1,0) , (1,0 ) and (0,1.5)*/
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
      wedgebd.position.Set(12.0f, 34.0f);/// let its position be (30,0) in world coordinates i.e it is on the ground
      sbody = m_world->CreateBody(&wedgebd);/// create the body using wedgebd
      sbody->CreateFixture(&wedgefd);/// add the fixture wedgefd to sbody  ( the vertices of wedgefd will have coordinates wrt sbody )
	
	///<br>
      /// 10. b)<b>The plank on top of the wedge</b>
      //The plank on top of the wedge
      b2PolygonShape shape;/// shape is a polygon shape we will create of type  b2PolygonShape
      shape.SetAsBox(11.0f, 0.1f);/// it is a 15 X 0.2 rectangle 
      b2BodyDef bd2;/// define a b2BodyDef variable bd2 which will be used to create the plank
      bd2.position.Set(12.0f, 37.0f);/// let its position be (30,1.5) in world coordinates i.e it touches the upper vertex of wedge.
      bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
      b2Body* body = m_world->CreateBody(&bd2);/// create body ( type  b2Body) using bd2
      b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable fd2 to create a fixture for body
      fd2->density = 0.5f;/// set its density to 1
      fd2->shape = new b2PolygonShape;
      fd2->shape = &shape;/// set its shape to the shape variable defined above
      body->CreateFixture(fd2); /// add the fixture fd2 to body
	
	///<br> 
      /** 10. c) <b>create a revolute joint at the point where the wedge and the plank meet for the see-saw.</b>*/
      b2RevoluteJointDef jd;///define a revolute (revolving) joint jd of type b2RevoluteJointDef
      b2Vec2 anchor;/// create a 2d array using b2vec2 called anchor to store location of anchor of point
      anchor.Set(12.0f, 37.0f);/// set coordinates of anchor to (30,1.5)
      jd.Initialize(sbody, body, anchor);/// initialise jd : define the 2 bodies (sbody and body) and location of common anchor
      jd.enableLimit = true;/// limit the rotation of the joint
  	jd.lowerAngle = -0.707;
  	jd.upperAngle =  0.707;
      m_world->CreateJoint(&jd);/// create the joint using jd
	
	// ball on left side of see saw
	b2Body* spherebody1;
	b2CircleShape circle1;
	circle1.m_radius = 1.0;
	b2FixtureDef ballfd1;
      ballfd1.shape = &circle1;
      ballfd1.density = 0.4f;
      ballfd1.friction = 0.1f;
      ballfd1.restitution = 0.0f;
      b2BodyDef ballbd1;
	ballbd1.type = b2_dynamicBody;
	ballbd1.position.Set(5.0f,38.0f );
	spherebody1 = m_world->CreateBody(&ballbd1);
	spherebody1->CreateFixture(&ballfd1);
	
	//ball on right side of see saw
	ballbd1.position.Set(19.0f,38.0f);
	spherebody1 = m_world->CreateBody(&ballbd1);
	spherebody1->CreateFixture(&ballfd1);
	
	// vertical obstacle which prevents ball on left from falling down to the right.
	b2PolygonShape shape2;
      shape2.SetAsBox(2.0f, 0.01f, b2Vec2(0.f,0.f), 1.57f);
	
      b2BodyDef bd4;
      bd4.position.Set(12.0f, 39.2f);
      b2Body* ground2 = m_world->CreateBody(&bd4);
      ground2->CreateFixture(&shape2, 0.0f);
      
    } 
    
    // balls at the bottom level
    {
	
	// balls 
	b2Body* spherebody;
	b2CircleShape circle;
	circle.m_radius = 1.0;
	b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 0.2f;
      ballfd.friction = 0.01f;
      ballfd.restitution = 0.0f;
      for (int i = 0; i < 4; ++i){
    		b2BodyDef ballbd;
		ballbd.type = b2_dynamicBody;
		
		ballbd.position.Set(36.0f - 10.0f * i,1.0f );
		spherebody = m_world->CreateBody(&ballbd);
		spherebody->CreateFixture(&ballfd);
	}
		
		// test ball 
//		b2BodyDef ballbd;
//		ballbd.type = b2_dynamicBody;
	
//		ballbd.position.Set(2.0f,23.0f );
//		spherebody = m_world->CreateBody(&ballbd);
//		spherebody->CreateFixture(&ballfd);
		
	
    }
    
    // pulley near the bottom
    {
   	b2BodyDef *bd = new b2BodyDef;/// b2BodyDef* type variable bd which describes the 2 bodies
      bd->type = b2_dynamicBody;/// Both the bodies are dynamic(i.e they can move)
      bd->position.Set(-16.0f,6.0f);/// first body is set at position (-10,15)
      bd->fixedRotation = true;/// bodies not allowed to rotate
      
      ///<br>
      /// 7. <b>a) The open box</b>
      //The open box
      b2FixtureDef *fd1 = new b2FixtureDef;/// define a fixture fd1 ( type  b2FixtureDef*)
      fd1->density = 21.5;/// set density to 10
      fd1->friction = 0.5;/// set friction to 0.5
      fd1->restitution = 0.f;/// set restitution to 0
      fd1->shape = new b2PolygonShape;
      b2PolygonShape bs1;
      bs1.SetAsBox(4.0,0.1);
      fd1->shape = &bs1;/// set fd1's shape member to bs1
      
      b2FixtureDef *fd2 = new b2FixtureDef;/// define another fixture fd2 ( type  b2FixtureDef*)
      fd2->density = 20.0;/// set density to 10
      fd2->friction = 0.5;/// set friction to 0.5
      fd2->restitution = 0.f;/// set restitution to 0
      fd2->shape = new b2PolygonShape;
      b2PolygonShape bs2;
      bs2.SetAsBox(0.1,1.0, b2Vec2(4.0f,1.0f), 0);/// create a shape bs ( type b2PolygonShape) which is a 0.4 X 4 rectangle centred at (2,0) and rotated by 0 rad 
      fd2->shape = &bs2;/// set fd2's shape member to bs2
      b2FixtureDef *fd3 = new b2FixtureDef;/// another fixture definition fd3 ( type  b2FixtureDef*)
      fd3->density = 20.0;/// set density to 10
      fd3->friction = 0.5;/// set friction to 0.5
      fd3->restitution = 0.f;/// set restitution to 0
      fd3->shape = new b2PolygonShape;
      b2PolygonShape bs3;
      bs3.SetAsBox(0.1,1, b2Vec2(-4.0f,1.0f), 0);/// bs3 is a 0.4 X 4 rectangle centred at (-2,0) and rotated by 0 rad 
      fd3->shape = &bs3;/// set fd3's shape member to bs3
       
      b2Body* box1 = m_world->CreateBody(bd);/// create the first body box1 (of type b2Body*) using bd which is an open box.
      box1->CreateFixture(fd1);/// add fixture fd1 to box1
      box1->CreateFixture(fd2);/// add fixture fd2 to box1
      box1->CreateFixture(fd3);/// add fixture fd3 to box1 (coordinates of the fixtures are wrt box1)
	
	///<br>
      /// 7.<b> b) The domino</b>
      //The domino
      bd->position.Set(-8.5f,1.5f);
      b2PolygonShape bs4;
      bs4.SetAsBox(0.2 , 1.5 , b2Vec2( 0.0f ,0.f) , 0);
      fd1->shape = &bs4;
      fd1->density = 42.1;/// set density of fd1 (fixture created earlier) to 34. this ensures second body of pulley is heavier.	  
      b2Body* box2 = m_world->CreateBody(bd);///create the second body box2 (of type b2Body*) using bd 
      box2->CreateFixture(fd1);/// add fixture fd1 to box2
	
	///<br>
      /// <b>The pulley joint</b>
      //The pulley joint
      b2PulleyJointDef* myjoint = new b2PulleyJointDef();/// define a b2PulleyJointDef* type variable called myjoint to create pulley joint / pivot
      
      b2Vec2 worldAnchorGround1(-16, 9.5); // Anchor point for ground 1 in world axis
      b2Vec2 worldAnchorGround2(-8.5, 9.5); // Anchor point for ground 2 in world axis
      /** ratio is quantity such that (length1 + ratio* length2 = constant) where length1 and length2 are lengths of the ropes on 2 sides of

       pulley.*/
       /**It can cause one side of pulley to slide faster.*/
      float32 ratio = 1.0f;/// set ratio to 1
      /** initialise myjoint so that
      <ul>
      <li> box1 and box2 are the 2 bodies</li>
      <li> worldAnchorGround1 (-10,20) and worldAnchorGround2(10,20) are the ground anchors respectively</li>

      <li> box1->GetWorldCenter() and box2->GetWorldCenter() are the local anchors respectively.</li></ul> also pass ratio as an argument while initialising myjoint
      */
      myjoint->Initialize(box1, box2, worldAnchorGround1, worldAnchorGround2, box1->GetWorldCenter(), box2->GetWorldCenter(), ratio);
      m_world->CreateJoint(myjoint);/// create joint in b2World using myjoint
    }
    
    //balls on platform above lower pulley
    {
    	b2Body* spherebody;
	b2CircleShape circle;
	circle.m_radius = 1.0;
	b2FixtureDef ballfd;
      ballfd.shape = &circle;
      ballfd.density = 0.2f;
      ballfd.friction = 0.1f;
      ballfd.restitution = 0.0f;
      for (int i = 0; i < 2; ++i){
    		b2BodyDef ballbd;
		ballbd.type = b2_dynamicBody;
		
		ballbd.position.Set(-9.0f - 3.0f * i,12.0f );
		spherebody = m_world->CreateBody(&ballbd);
		spherebody->CreateFixture(&ballfd);
	}	
    }	
    
    // dominos on level 2 from bottom (which will topple on both sides)
    {
   	b2PolygonShape shape;
      shape.SetAsBox(0.5f, 2.5f);/// create a 0.2 X 2 rectangle called shape (type b2PolygonShape) 
	
      b2FixtureDef fd;/// define a fixture fd of type b2FixtureDef
      fd.shape = &shape;///set its shape to variable shape defined above
      fd.density = 0.5f;/// set its density to 20
      fd.friction = 0.1f;/// set friction to 0.1
		
      /** create 10 identical dominoes in the b2World separated by 3 unit along horizontal axis */ 
      for (int i = 0; i < 5; ++i)
	{
	  b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	  bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	  
	  /** bodies are at 31.25f along vertical axis and start from -35.5f onwards in horizontal axis*/
	  bd.position.Set(-5.5f + 1.25f * i, 8.5f);
	  b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  body->CreateFixture(&fd);/// add a fixture fd to the body
	}
	
	//create 2 dominoes on top of the 3 existing dominos
	b2PolygonShape shape1;
      shape1.SetAsBox(0.5f,2.0f);/// create a 0.2 X 2 rectangle called shape (type b2PolygonShape) 
      fd.shape = &shape1;
	for ( int i =0 ; i<2 ; i++)
	{
		b2BodyDef bd;/// b2BodyDef type variable bd which describes the body.
	 	bd.type = b2_dynamicBody;///body is dynamic i.e it can move 
	 	
	 	bd.position.Set( -4.5f + 3.0 * i , 13.0f);
	 	b2Body* body = m_world->CreateBody(&bd);/// create body of type b2Body* using bd
	  	body->CreateFixture(&fd);/// add a fixture fd to the body  
	}	
    }
    
    
    ///<br>
    /// 8. <b>The revolving horizontal platform</b>
    //The revolving horizontal platform
    {
      b2PolygonShape shape, shape2;  
      shape.SetAsBox(2.2f, 0.2f);///<br> define a 4.4 X 0.4 rectangular shape called shape (b2PolygonShape type)
	  shape2.SetAsBox(2.2f, 0.2f);
	
      b2BodyDef bd, bd3; /// define a b2BodyDef variable bd which will be used to create the platform 
      bd.position.Set(38.0f, 40.0f);/// set its position as (14,14)
	  bd3.position.Set(28.0f, 29.0f);
      bd.type = b2_dynamicBody;  /// let it be dynamic i.e it can move
 	  bd3.type = b2_dynamicBody;
      b2Body* body = m_world->CreateBody(&bd);/// create a b2Body* variable body which Refers to the platform.
      b2Body* body3 = m_world->CreateBody(&bd3);
      b2FixtureDef *fd = new b2FixtureDef; /// define a fixture fd 
      b2FixtureDef *fd2 = new b2FixtureDef; /// define a fixture fd 
      fd->density = 10.0f;/// set its density as 1
      fd2->density = 40.0f;
      fd->shape = new b2PolygonShape;
	  fd2->shape = new b2PolygonShape;
      fd->shape = &shape; /// set shape member of fd to the shape variable defined above 
	  fd2->shape = &shape2;
      body->CreateFixture(fd);/// add fixture fd to body
	  body3->CreateFixture(fd2);

  //    b2PolygonShape shape2;
   //   shape2.SetAsBox(0.2f, 2.0f);///<br> define a 0.4 X 4 rectangular shape called shape2 (b2PolygonShape type)
      /** define a b2BodyDef variable bd2 which will be used to create another body for the pivot/hinge of the platform*/
      b2BodyDef bd2;
      bd2.position.Set(25.0f, 28.0f);/// set its position as (14,16)
      b2Body* body2 = m_world->CreateBody(&bd2);/// body2 is of b2Body* type. Refers to a dummy body created for the hinge.

      b2RevoluteJointDef jointDef;///<br> create a  revolute (revolving) joint definition called jointDef
      jointDef.bodyA = body;/// its first body is body
      jointDef.bodyB = body2;/// its second body is body2
      jointDef.localAnchorA.Set(0,0);/// set the anchor for body as (0,0) wrt itself
      jointDef.localAnchorB.Set(0,0);/// set the anchor for body2 as (0,0) wrt itself
      jointDef.collideConnected = false;/// disallow collision 
      m_world->CreateJoint(&jointDef);/// create the joint using jointDef
    }

    
    //Creating wedges at the lower right corner for the see-saws
    
    for(int a = 0; a <= 4; a++)
    {

    see_saw_create( 4 + a*6, 8 , 2.85 , 0.1);
    
  	b2Body* spherebody;
	b2CircleShape circle;
	circle.m_radius = 0.7;
	b2FixtureDef ballfd;
    ballfd.shape = &circle;
    ballfd.density = 2.0f;
    ballfd.friction = 0.1f;
    ballfd.restitution = 0.0f;
    b2BodyDef ballbd;
	ballbd.type = b2_dynamicBody;
	ballbd.position.Set(7 + a*6,14);
	spherebody = m_world->CreateBody(&ballbd);
	spherebody->CreateFixture(&ballfd);
    
    
    b2PolygonShape shape, shape2;  
    shape.SetAsBox(0.3f, 0.3f);///<br> define a 4.4 X 0.4 rectangular shape called shape (b2PolygonShape type)
	
    b2BodyDef bd; /// define a b2BodyDef variable bd which will be used to create the platform 
    bd.position.Set(7 + a*6, 12.0f);/// set its position as (14,14)
    bd.type = b2_dynamicBody;  /// let it be dynamic i.e it can move
    b2Body* body = m_world->CreateBody(&bd);/// create a b2Body* variable body which Refers to the platform.
    b2FixtureDef *fd = new b2FixtureDef; /// define a fixture fd 
    fd->density = 10.0f;/// set its density as 1
    fd->shape = new b2PolygonShape;
    fd->shape = &shape; /// set shape member of fd to the shape variable defined above 
    body->CreateFixture(fd);/// add fixture fd to body

    
    b2BodyDef bd2;
    bd2.position.Set(7 + a*6, 12.0f);/// set its position as (14,16)
    b2Body* body2 = m_world->CreateBody(&bd2);/// body2 is of b2Body* type. Refers to a dummy body created for the hinge.

    b2RevoluteJointDef jointDef;///<br> create a  revolute (revolving) joint definition called jointDef
    jointDef.bodyA = body;/// its first body is body
    jointDef.bodyB = body2;/// its second body is body2
    jointDef.localAnchorA.Set(0,0);/// set the anchor for body as (0,0) wrt itself
    jointDef.localAnchorB.Set(0,0);/// set the anchor for body2 as (0,0) wrt itself
    jointDef.collideConnected = false;/// disallow collision 
    m_world->CreateJoint(&jointDef);/// create the joint using jointDef

 	}   


	//Creating the gate structure at left-mid section
	
    b2EdgeShape shape;
    shape.Set(b2Vec2(-41.0f,33.5f) , b2Vec2(-25.0f,33.5f));  
	
    b2BodyDef bd;/// b2BodyDef type variable bd which describes the body. It is static
    b2Body* ground = m_world->CreateBody(&bd);/// create a body (*ground) of type b2Body using bd
    ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density

	shape.Set(b2Vec2(-41.0f, 23.5f) , b2Vec2(-25.0f, 23.5f));
    ground->CreateFixture(&shape, 0.0f);/// add a fixture to the body with shape as defined above and having 0 density

    b2PolygonShape shape2, shape3;
    shape2.SetAsBox(2.5, 5);/// create a 0.2 X 2 rectangle called shape (type b2PolygonShape) 
	shape3.SetAsBox(2.5, 2.5);
	
    b2BodyDef bd2;/// define a b2BodyDef variable bd2 which will be used to create the plank
    bd2.position.Set(-27.5 , 28.5);/// let its position be (30,1.5) in world coordinates i.e it touches the upper vertex of wedge.
    bd2.type = b2_dynamicBody;/// let it be dynamic i.e it can move
    b2Body* body = m_world->CreateBody(&bd2);/// create body ( type  b2Body) using bd2
    b2FixtureDef *fd2 = new b2FixtureDef;/// define a b2FixtureDef* variable fd2 to create a fixture for body
    fd2->density = 0.0001f;/// set its density to 1
    fd2->shape = new b2PolygonShape;
    fd2->shape = &shape2;/// set its shape to the shape variable defined above
    body->CreateFixture(fd2); /// add the fixture fd2 to body

	bd2.position.Set( -27.5 , 2.5);
	b2Body* body2 = m_world->CreateBody(&bd2);
	body2 = m_world->CreateBody(&bd2);	
	fd2->density = 0.01f;
	fd2->shape = &shape3;
	body2->CreateFixture(fd2);
	    
	b2DistanceJointDef* dist = new b2DistanceJointDef();
	dist->Initialize(body, body2, body->GetWorldCenter(), body2->GetWorldCenter());
	m_world->CreateJoint(dist);
   
   
   
    
  }

  sim_t *sim = new sim_t("Dominos", dominos_t::create);
}
