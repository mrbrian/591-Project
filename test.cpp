
int shadeTest()
{
    qDebug() << "shadetest";

    Vector3D n;
    Colour color;
    Scene sc;

    Point3D o = Point3D(0,0,-1);

    Colour difClr = Colour(1,0.5,0.5);
    Material *mat = new Material(Colour(0,0,0), difClr, Colour(1,1,1), 1);

    Light *light = new Light(Point3D(0,2,2), Colour(1,1,1), Colour(1,1,1), Colour(1,1,1));
    Sphere *sphere = new Sphere(Point3D(0,0,2), 1, mat);
    sc.objects.push_back(sphere);
    sc.lights.push_back(light);

    Vector3D dir = Point3D(0,1,2) - o;
    dir.normalize();

    qDebug() << dir[0] << dir[1] << dir[2];
    double t = sphere->intersect(o, dir, &n);
    qDebug() << t;
    trace_ray(Point3D(0,0,-1), dir, sc, &color, 1);

    qDebug() << color.R() << color.G() << color.B();
    if (color.R() != difClr.R())
        return 1;
    if (color.G() != difClr.G())
        return 1;
    if (color.B() != difClr.B())
        return 1;
    return 0;
}

int shadowTest(int x, int y)
{
    qDebug() << "shadowtest" << x << y;
    int width  = 50;
    int height = 50;

    Scene scene = Scene();
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-1);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    Material *mat = new Material(Colour(1,1,1), Colour(1,1,1), Colour(1,1,1), 1);
    Sphere *sphere = new Sphere(Point3D(0,-1,2), 1, mat);
    Sphere *sphere2 = new Sphere(Point3D(0,1,2), 0.1, mat);

    scene.objects.push_back(sphere);
    scene.objects.push_back(sphere2);

    Light *light = new Light(Point3D(0,12,2), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);

    Point3D p = ImageToWorldSpace(x, y, width, height, planeC, planeW, planeH);
    Vector3D v = p - cam.position;
    v.normalize();

    Colour clr;
    trace_ray(cam.position, v, scene, &clr, 1);
    Vector3D n;
    //double t = sphere2.intersect(cam.position, v, &n);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    return 0;
}

void Test(Vector3D *v)
{
    Vector3D &b = *v;
    b[0] = 5;
}

int classTest()
{
    Vector3D v;
    Test(&v);
    if (v[0] != 5)
        return 1;
    return 0;
}

int triIntTest2(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene = Scene();
    Camera cam = Camera();

    cam.imgWidth = width;
    cam.imgHeight = height;
    cam.lookAt = Point3D(0,0,1);
    cam.position = Point3D(0,0,-12);

    scene.cam = cam;

    Point3D planeC;
    float planeW = 1;
    float planeH = 1;

    Material *mat = new Material(Colour(1,1,1), Colour(1,1,1), Colour(1,1,1), 1);
    Triangle *tri = new Triangle(Point3D(0,1,1), Point3D(0,0,1), Point3D(1,0,1), mat);

    scene.objects.push_back(tri);

    Light *light = new Light(Point3D(0,2,0), Colour(0.1,0.1,0.1), Colour(1,1,1), Colour(1,1,1));
    scene.lights.push_back(light);

    Point3D p = ImageToWorldSpace(x, y, width, height, planeC, planeW, planeH);
    Vector3D v = p - cam.position;
    Vector3D n;
    Colour clr;
    trace_ray(cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.R() == 0 && clr.G() == 0 && clr.B() == 0)
        return 0;
    return 1;
}


int triIntTest(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.R() == 1 && clr.G() == 1 && clr.B() == 1)
        return 0;
    return 1;
}

int reflectTest(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene_RefTest(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.R() == 0.1 && clr.G() == 0.1 && clr.B() == 0.1)
        return 1;
    return 1;
}


int reflectTest2(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene_RefTest(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.R() == 0.1 && clr.G() == 0.1 && clr.B() == 0.1)
        return 1;
    return 0;
}

int reflectTest3(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene_RefTest(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.G() >= 0.42)
        return 1;
    if (clr.R() <= 0.33)
        return 0;
    return 1;
}

int reflectTest4(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene_RefTest2(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.G() <= 0.1)
        return 1;
    if (clr.R() <= 0.1)
        return 1;
    return 0;
}

int reflectTest5(int x, int y)
{
    qDebug() << "triIntTest" << x << y;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;

    Scene scene;
    SetupScene_RefTest2(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    trace_ray(scene.cam.position, v, scene, &clr, 1);
    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.G() <= 0.5)
        return 0;
    return 1;
}

int checkerTest(int x, int y)
{
    Scene scene;
    int width  = DEF_WIDTH;
    int height = DEF_HEIGHT;
    SetupScene_Checker2(&scene, width, height);

    Point3D p = ImageToWorldSpace(x, y, width, height, scene.cam.position, scene.imgPlane->width, scene.imgPlane->height);
    Vector3D v = p - scene.cam.position;
    v.normalize();
    Colour clr;

    scene.debug = true;
    trace_ray(scene.cam.position, v, scene, &clr, 1);

    qDebug() << "outcolor:" << clr.R() << clr.G() << clr.B();
    // create new image
    if (clr.G() >= 0.5)
        return 1;
    return 0;
}

int testing()
{/*
    if (checkerTest(289,344))
    {
        qDebug() << "checkertest fail";
        return 1;
    }

    if (reflectTest5(119,115))
        {
            qDebug() << "reflectTest5 fail";
            return 1;
        }
    if (reflectTest4(1,100))
        {
            qDebug() << "reflectTest4 fail";
            return 1;
        }
    if (reflectTest3(117,100))
    {
        qDebug() << "reflectTest3 fail";
        return 1;
    }

    if (reflectTest2(70,100))
    {
        qDebug() << "reflectTest2 fail";
        return 1;
    }
    if (reflectTest(125,100))
    {
        qDebug() << "reflectTest fail";
        return 1;
    }
    if (triIntTest(118,52))
    {
        qDebug() << "triIntTest fail";
        //return 1;
    }
    if (classTest())
    {
        qDebug() << "ugh class test fail";
        return 1;
    }

    //Point3D p = ImageToWorldSpace(x, y, width, height, planeC, planeW, planeH);
    Vector3D n;
    Colour difClr = Colour(1,0.5,0.5);
    Material *mat = new Material(Colour(0,0,0), difClr, Colour(1,1,1), 1);
    Sphere s = *new Sphere(Point3D(0,0,2), 1, mat);
    double t = s.intersect(Point3D(), Vector3D(0,0,1), &n);

    qDebug() << t;
    if (t != 1)
        return 1;

    t = s.intersect(Point3D(0,0,-1), Vector3D(0,0,1), &n);

    qDebug() << t;

    if (t != 2)
        return 1;

    s.position = Point3D(0,-1,2);
    t = s.intersect(Point3D(0,0,-1), Vector3D(0,0,1), &n);

    qDebug() << t;

    if (t != 3)
        return 1;

    s.radius = 1;
    s.position = Point3D(0,0,2.5);
    t = s.intersect(Point3D(0,0,-1), Vector3D(0,0,1), &n);

    Colour color;
    Scene scn;
    scn.objects.push_back(new Sphere(Point3D(0,0,2), 1, mat));
    scn.lights.push_back(new Light(Point3D(0,2,2), Colour(1,1,1), Colour(1,1,1), Colour(1,1,1)));

    Vector3D dir = Point3D(0,2,2) - Point3D(0,0,-1);
    trace_ray(Point3D(0,0,-1), dir, scn, &color, 1);

    //qDebug() << color.R() << color.G() << color.B();
    //qDebug() << t;

    if (t != 2.5)
        return 1;

    if (shadowTest(25, 7))
    {
        qDebug() << "shadow test fail?";
    }

    if (shadowTest(14, 30))
    {
        qDebug() << "shadow test fail?";
    }

    if (shadeTest())
    {
        qDebug() << "shade test fail?";
    }
*/
    return 0;
}
