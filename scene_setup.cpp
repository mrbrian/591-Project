// nice chrome 3 ball reflection
void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,8);
    cam.position = Point3D(0,0,0);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,1), Point3D(-0.5,-0.5,1), Point3D(0.5,-0.5,1), Point3D(0.5,0.5,1));

    Material *checker = new ChromeMaterial();//Colour(0,0,0), Colour(0.2,0.2,0.2), Colour(0.5,0.5,0.5), 100,  Colour(0.4,0.4,0.4), 0.4);
   /* checker->Ka = Colour(.1,.1,.1);
    checker->Kd = Colour(0,0.65,0.24);
    checker->Ks = Colour(0,0,0);
    checker->Kr = Colour(0,0,0);*/
    Material *mat1   = new Material(Colour(0,0,0), Colour(1,0.25,0.1), Colour(1,1,1), 1000,  Colour(0.5,0.5,0.5));
    Material *mat2 = new Material(Colour(0,0,0), Colour(0.05,0.6,0.95), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *mat3  = new Material(Colour(0,0,0), Colour(0.7,0.95,.13), Colour(1,1,1), 10, 0.5 * Colour(0.7,0.95,.13));

    Quad *q = new Quad(Point3D(-1,0,1),Point3D(-1,0,-1),Point3D(1,0,-1),Point3D(1,0,1), checker);
    q->Transform( Matrix4x4::translation(Vector3D(0,-1,0)) * Matrix4x4::scaling(Vector3D(2000,2000,2000)));
    Sphere *sphere1 = new Sphere(Point3D(-2,0,6), 1, mat1);
    Sphere *sphere2 = new Sphere(Point3D(0,0,10), 1, mat2);
    Sphere *sphere3 = new Sphere(Point3D(2,0,6), 1, mat3);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);
    scene.objects.push_back(q);

    Light *light = new Light(Point3D(0,5,0), Colour(0.1,0.1,0.1), Colour(0.5,0.5,0.5), Colour(0,0,0));
    scene.lights.push_back(light);
    Light *light2 = new Light(Point3D(10,15,1), Colour(0.1,0.1,0.1), Colour(0.6,0.6,0.5), Colour(1,1,1));
    scene.lights.push_back(light2);
    Light *light3 = new Light(Point3D(-20,20,40), Colour(0.1,0.1,0.1), Colour(0.25,0.25,0.35), Colour(1,1,1));
    scene.lights.push_back(light3);

    scene.Transform(
                Matrix4x4::translation(Vector3D(cam.lookAt)) *
                Matrix4x4::rotation(30*M_PI/180,'y') *
                Matrix4x4::rotation(-35*M_PI/180,'x') *
                Matrix4x4::translation(-Vector3D(cam.lookAt))
                    );
}


// pretty tri ball
void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-0.5,0.5,0), Point3D(-0.5,-0.5,0), Point3D(0.5,-0.5,0), Point3D(0.5,0.5,0));

    Material *checker = new CheckerMaterial(Colour(0,0,0), Colour(0.2,0.2,0.2), Colour(0.5,0.5,0.5), 100,  Colour(0.4,0.4,0.4), 0.4);
    Material *red   = new Material(Colour(0,0,0), Colour(1,0,0), Colour(1,1,1), 1000,  Colour(0.5,0.5,0.5));
    Material *green = new Material(Colour(0,0,0), Colour(0,1,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *blue  = new Material(Colour(0,0,0), Colour(0,0,1), Colour(1,1,1), 10,  Colour(0.5,0.5,0.5));

    Quad *q1 = new Quad(Point3D(-10,0,10), Point3D(-10,0,-10), Point3D(10,0,-10), Point3D(10,0,10), checker);
    Matrix4x4 m = Matrix4x4::translation(Vector3D(0,-1,0)) * Matrix4x4::scaling(Vector3D(2,2,2));
    q1->Transform(m);
    scene.objects.push_back(q1);

    Sphere *sphere1 = new Sphere(Point3D(-2,0,5), 1,red);
    Sphere *sphere2 = new Sphere(Point3D(0,0,6), 1, green);
    Sphere *sphere3 = new Sphere(Point3D(2.5,0,7), 1, blue);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(0,2,0), Colour(0.1,0.1,0.1), Colour(0.5,0.5,0.5), Colour(0,0,0));
    scene.lights.push_back(light);
    Light *light2 = new Light(Point3D(10,15,1), Colour(0.1,0.1,0.1), Colour(0.6,0.6,0.5), Colour(1,1,1));
    scene.lights.push_back(light2);
    Light *light3 = new Light(Point3D(-20,20,40), Colour(0.1,0.1,0.1), Colour(0.25,0.25,0.35), Colour(1,1,1));
    scene.lights.push_back(light3);
}



// meh scene   2 balls.  
void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    scene.imgPlane = new Plane(Point3D(-0.5,0.5,0), Point3D(0.5,-0.5,0), Point3D(0.5,-0.5,0), Point3D(0.5,0.5,0));

    Material *black = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100, Colour(1,1,1));
    Material *white = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Quad *q1 = new Quad(Point3D(0,0,0), Point3D(0,0,-1), Point3D(1,0,-1), Point3D(1,0,0), black);
    Quad *q2 = new Quad(Point3D(-1,0,0), Point3D(-1,0,-1), Point3D(0,0,-1), Point3D(0,0,0), white);
    Quad *q3 = new Quad(Point3D(-1,0,1), Point3D(-1,0,0), Point3D(0,0,0), Point3D(0,0,1), black);
    Quad *q4 = new Quad(Point3D(0,0,1), Point3D(0,0,0), Point3D(1,0,0), Point3D(1,0,1), white);

    Matrix4x4 scale = Matrix4x4::scaling(Vector3D(4,4,4));
    Matrix4x4 trans = Matrix4x4::translation(Vector3D(0, -1, 5));
    q1->Transform(trans * scale);
    q2->Transform(trans * scale);
    q3->Transform(trans * scale);
    q4->Transform(trans * scale);

    Sphere *sphere2 = new Sphere(Point3D(0,0.5,5), 1, black);
    Sphere *sphere3 = new Sphere(Point3D(2,0,5), 0.5, white);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);
    scene.objects.push_back(q1);
    scene.objects.push_back(q2);
    scene.objects.push_back(q3);
    scene.objects.push_back(q4);

    Light *light = new Light(Point3D(0,2,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
   // scene.lights.push_back(light);

    Light *light2 = new Light(Point3D(0,5,5), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light2);
}


void *SetupScene_Checker2(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    double planeW = 1;
    double planeH = 1;

    scene.imgPlane = new ImagePlane(planeC, planeW, planeH);

    Material *checker = new CheckerMaterial(Colour(0,0,0), Colour(0,0,0), Colour(1,1,1), Colour(1,1,1), 100,  Colour(0.4,0.4,0.4),  0.2);
    Material *red   = new Material(Colour(0,0,0), Colour(0,0,0), Colour(1,0,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *green = new Material(Colour(0,0,0), Colour(0,0,0), Colour(0,1,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *blue  = new Material(Colour(0,0,0), Colour(0,0,0), Colour(0,0,1), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));

    Quad *q1 = new Quad(Point3D(-10,0,10), Point3D(-10,0,-10), Point3D(10,0,-10), Point3D(10,0,10), checker);
    Matrix4x4 m = Matrix4x4::translation(Vector3D(0,-1,0));
    q1->Transform(m);
    scene.objects.push_back(q1);

    Sphere *sphere1 = new Sphere(Point3D(-2,0,5), 1,red);
    Sphere *sphere2 = new Sphere(Point3D(0,0,5), 1, green);
    Sphere *sphere3 = new Sphere(Point3D(2,0,5), 1, blue);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(0,2,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
    Light *light2 = new Light(Point3D(0,5,1), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light2);
}

void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    scene.imgPlane = new ImagePlane(planeC, planeW, planeH);
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Sphere *sphere = new Sphere(Point3D(0,-1,2), 1, redMat);
    Sphere *sphere2 = new Sphere(Point3D(0,0.75,1.8), 0.5, blueMat);
    Triangle *tri = new Triangle(Point3D(0,1,1), Point3D(0,0,1), Point3D(1,0,1), redMat);

    //scene.objects.push_back(sphere);
    //scene.objects.push_back(&sphere2);
    scene.objects.push_back(tri);

    Light *light = new Light(Point3D(0,0,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
}

void *SetupScene_RefSpheres(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    scene.imgPlane = new ImagePlane(planeC, planeW, planeH);
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Sphere *sphere  = new Sphere(Point3D(-1,0,2), 1, redMat);
    Sphere *sphere2 = new Sphere(Point3D(1,0,2), 1, blueMat);

   scene.objects.push_back(sphere);
   scene.objects.push_back(sphere2);

    Light *light = new Light(Point3D(0,0,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    Light *light2 = new Light(Point3D(2,1,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
    scene.lights.push_back(light2);
}

void *SetupScene_RefTest(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    scene.imgPlane = new ImagePlane(planeC, planeW, planeH);
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Matrix4x4 rot   = Matrix4x4::rotation(M_PI_2, 'y');
    Matrix4x4 trans = Matrix4x4::translation(Vector3D(0.5,0,5));
    Matrix4x4 scale = Matrix4x4::scaling(Vector3D(10,10,10));

    Quad *q1    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), redMat);
    q1->Transform(trans * rot * scale);

    /*trans = Matrix4x4::translation(Vector3D(-1,0,2));
    Quad *q2    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), blueMat);
    q2->Transform(trans * rot * scale);
    scene.objects.push_back(q2);
*/
    scene.objects.push_back(q1);

    Sphere *sphere2 = new Sphere(Point3D(0,0,5), 1, blueMat);
    scene.objects.push_back(sphere2);

    Light *light = new Light(Point3D(0,0,-2), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
}

void *SetupScene_RefTest2(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    scene.imgPlane = new ImagePlane(planeC, planeW, planeH);
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Matrix4x4 rot   = Matrix4x4::rotation(M_PI_2, 'y');
    Matrix4x4 trans = Matrix4x4::translation(Vector3D(0.5,0,5));
    Matrix4x4 scale = Matrix4x4::scaling(Vector3D(10,10,10));

    Quad *q1    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), redMat);
    q1->Transform(trans * rot * scale);

    trans = Matrix4x4::translation(Vector3D(-1,0,2));
    Quad *q2    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), blueMat);
    q2->Transform(trans * rot * scale);
    //scene.objects.push_back(q2);
    scene.objects.push_back(q1);
    Sphere *sphere3 = new Sphere(Point3D(-2,0,5), 1, blueMat);
    Sphere *sphere2 = new Sphere(Point3D(0,0,5), 1, blueMat);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(0,0,-2), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
}

// square  + balls 
void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    scene.imgPlane = new Plane(Point3D(-0.5, 0.5, 0), Point3D(-0.5, -0.5, 0), Point3D(0.5, 0.5, 0), Point3D(0.5, 0.5, 0));
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Sphere *sphere  = new Sphere(Point3D(0,-1,2), 1, redMat);
    Sphere *sphere2 = new Sphere(Point3D(0,0.75,1.8), 0.5, blueMat);
    Triangle *tri   = new Triangle(Point3D(0,1,1), Point3D(0,0,1), Point3D(1,0,1), redMat);
    Quad *q         = new Quad(Point3D(1,1,1), Point3D(0,1,1), Point3D(0,0,1), Point3D(1,0,1), redMat);

   //scene.objects.push_back(sphere);
   //scene.objects.push_back(sphere2);
   //scene.objects.push_back(tri);
    scene.objects.push_back(q);

    Light *light = new Light(Point3D(0,0,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    Light *light2 = new Light(Point3D(2,1,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);
    scene.lights.push_back(light2);
}


// just a quad in the middle
void *SetupSceneTriTest(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    //scene.imgPlane = new ImagePlane(planeC, planeW, planeH);
    scene.imgPlane = new Plane(Point3D(-1,1,0),Point3D(-1,-1,0),Point3D(1,-1,0),Point3D(1,1,0));
    Material *redMat = new Material(Colour(1,1,1), Colour(1,0.2,0.2), Colour(1,1,1), 100);
    Material *blueMat = new Material(Colour(1,1,1), Colour(0.2,0.2,1), Colour(1,1,1), 100);

    Matrix4x4 rot   = Matrix4x4::rotation(M_PI_2, 'y');
    Matrix4x4 scale = Matrix4x4::scaling(Vector3D(10,10,10));
    Matrix4x4 trans = Matrix4x4::translation(Vector3D(0,0,10));

    Quad *q1    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), redMat);
    q1->Transform(trans);

    trans = Matrix4x4::translation(Vector3D(-1,0,2));
    Quad *q2    = new Quad(Point3D(1,1,0), Point3D(-1,1,0), Point3D(-1,-1,0), Point3D(1,-1,0), blueMat);
    q2->Transform(trans * rot * scale);
//    scene.objects.push_back(q2);
    scene.objects.push_back(q1);
    Sphere *sphere3 = new Sphere(Point3D(-2,0,5), 1, blueMat);
    Sphere *sphere2 = new Sphere(Point3D(0,0,5), 1, blueMat);
   // scene.objects.push_back(sphere2);
  //  scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(0,0,-2), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);

    Point3D p = scene.imgPlane->ImageToWorldSpace(0,0,width,height);
    Point3D p1 = scene.imgPlane->ImageToWorldSpace(125,250,width,height);
    Point3D p2 = scene.imgPlane->ImageToWorldSpace(200,300,width,height);
    qDebug() << "imgtowrld " << p2[0] << p2[1] << p2[2];

    Colour c;
    Vector3D v = scene.imgPlane->ImageToWorldSpace(200,300, width, height) - cam.position;
    scene.debug = true;
    trace_ray(cam.position, v, &scene, &c, 1);
    if (c.R() == 0)
        qDebug() << "Fail";
    else
        qDebug() << "Pass";
    scene.debug = false;
}


// mirrors + 3 balls
void *SetupScene(Scene *s, int width, int height)
{
    Scene &scene = *s;
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;
    scene.imgPlane = new Plane(Point3D(-1,1,0),Point3D(-1,-1,0),Point3D(1,-1,0),Point3D(1,1,0));
    scene.imgPlane->Transform(Matrix4x4::scaling(Vector3D(0.5, 0.5, 0.5)));

    Material *checker = new CheckerMaterial(Colour(0,0,0), Colour(.5,.5,.5), Colour(1,1,1), 100,  Colour(0.4,0.4,0.4),  2);
    Material *red   = new Material(Colour(0,0,0), Colour(1,0,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *green = new Material(Colour(0,0,0), Colour(0,1,0), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));
    Material *blue  = new Material(Colour(0,0,0), Colour(0,0,1), Colour(1,1,1), 100,  Colour(0.5,0.5,0.5));

    Quad *q1 = new Quad(Point3D(-10,0,10), Point3D(-10,0,-10), Point3D(10,0,-10), Point3D(10,0,10), checker);

    q1->Transform(Matrix4x4::translation(Vector3D(0,-4,0)) *
                  Matrix4x4::scaling(Vector3D(4,4,4)) *
                  Matrix4x4::rotation(M_PI_4,'y'));

    scene.objects.push_back(q1);

    Quad *q2 = new Quad(Point3D(-10,5,0), Point3D(-10,-4,-0), Point3D(0,-4,10), Point3D(0,5,10), new ChromeMaterial());
    scene.objects.push_back(q2);

    Quad *q3 = new Quad(Point3D(0,5,10), Point3D(0,-4,10), Point3D(10,-4,0), Point3D(10,5,0), new ChromeMaterial());
    scene.objects.push_back(q3);

    Sphere *sphere1 = new Sphere(Point3D(0,1.8,5), 0.3,red);
    Sphere *sphere2 = new Sphere(Point3D(0,0.7,5), 0.6, green);
    Sphere *sphere3 = new Sphere(Point3D(0,-2,5), 2, blue);
    scene.objects.push_back(sphere1);
    scene.objects.push_back(sphere2);
    scene.objects.push_back(sphere3);

    Light *light = new Light(Point3D(10,10,0), Colour(1,1,1), Colour(1,1,0.9), Colour(1,1,1));
    scene.lights.push_back(light);

    Light *light2 = new Light(Point3D(-5,10,20), Colour(0.1,0.1,0.12), Colour(0.6,1,1), Colour(1,1,1));
    scene.lights.push_back(light2);

    scene.Transform(Matrix4x4::rotation(-10.0*M_PI/180.0, 'x'));
}


class Cube : public SceneObject
{
public:
    Quad **faces;    // array of Quad pointers

    char const *type()
    {
        return "Cube";
    }

    Cube(Point3D center, float scale, Material *mat)
    {
        faces = new Quad*[6];
        material = mat;
        for (int i = 0; i < 4; i++)
        {
            Quad *q = new Quad(Point3D(-1,1,1), Point3D(-1,-1,1), Point3D(1,-1,1), Point3D(1,1,1), material);
            faces[i] = q;
            q->Transform(Matrix4x4::rotation(M_PI_2 * i, 'y'));
        }

        faces[4] = new Quad(Point3D(-1,1,1), Point3D(-1,1,-1), Point3D(1,1,-1), Point3D(1,1,1), material);
        faces[5] = new Quad(Point3D(-1,-1,1), Point3D(-1,-1,-1), Point3D(1,-1,-1), Point3D(1,-1,1), material);
        Transform(
                    Matrix4x4::translation(Vector3D(center[0], center[1], center[2])) *
                    Matrix4x4::scaling(Vector3D(scale,scale,scale)));
    }

    void Transform(Matrix4x4 m)
    {
        for (int j = 0; j < 6; j++)
        {
            faces[j]->Transform(m);
        }
    }

    double intersect(Point3D o, Vector3D v, Vector3D *n)
    {
        double t_min = INFINITY;
        for (int i = 0 ; i < 6; i++)
        {
            double t = faces[i]->intersect(o,v,n);
            if (t <= t_min && t >= 0)
                t_min = t;
        }
        if (t_min == INFINITY)
            return -1;
        return t_min;
    }
};
