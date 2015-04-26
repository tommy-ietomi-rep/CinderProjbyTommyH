//Output Movie parameters

const int           WIN_WIDTH     = 600;
const int           WIN_HEIGHT    = 600;

const int           MOVIE_WIDTH     = 2000;
const int           MOVIE_HEIGHT    = 2000;
const bool          MOVIE_ON        = false;
const float         MOVIE_FPS       = 30.0f;
const char          MOVIE_PATH[100] = "F:\\GenArt\\ColorParticles\\ColorParticles.mov";
const uint32_t      MOVIE_FOUR_CC   = 'avc1';//H.264 'tiff';//no compress
const float         MOVIE_QUOLITY   = 0.9f;

//image path
const char          IMAGE_PATH[100]    = "F:\\GenArt\\ColorParticles\\ColorParticles";
const char          IMAGE_EXT[100]     = "tiff";

//Back Ground Color Settings
const float         BG_COL_R = 0.5*0.9;
const float         BG_COL_G = 0.5*0.9;
const float         BG_COL_B = 1.0*0.9;

//test
const int PERLIN_SEED[20]     = {111,222,333,444,555,666,777,888,9,71,84,87,87,4,15,41,541,5415,541};
const int PERLIN_OCT[20]      = {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};


//type
class MyParticle1;
typedef MyParticle1 GenWrokClass;
