#include <surf_matcher.hpp>
#include <opencv2/features2d/features2d.hpp>

#ifdef RMB_NON_FREE
#include <opencv2/nonfree/features2d.hpp>
#endif

#include <vector>

namespace Roombara
{

cv::Rect SurfMatcher::DoMatch(const cv::Mat& object, cv::Mat& scene)
{
	cv::Rect foundRect;
	
    float nndrRatio = 0.7f;
    //vector of keypoints   
    std::vector< cv::KeyPoint > keypointsO;
    std::vector< cv::KeyPoint > keypointsS;    

    cv::Mat descriptors_object, descriptors_scene;      

    //-- Step 1: Extract keypoints
    cv::SurfFeatureDetector surf(m_hessianValue); 
    //  FastFeatureDetector surf(m_hessianValue);
    surf.detect(scene,keypointsS);
    if(keypointsS.size() < 3) return foundRect; //Not enough keypoints, object not found
    surf.detect(object,keypointsO);
    if(keypointsO.size() < 4) return foundRect; //Not enough keypoints, object not found

    //-- Step 2: Calculate descriptors (feature vectors)
    cv::SurfDescriptorExtractor extractor;
    //  BriefDescriptorExtractor extractor(32);
    extractor.compute( scene, keypointsS, descriptors_scene );
    extractor.compute( object, keypointsO, descriptors_object );

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    cv::FlannBasedMatcher matcher;  
    //BruteForceMatcher< L2<float> > matcher;

    std::vector< std::vector< cv::DMatch >  > matches;
    matcher.knnMatch( descriptors_object, descriptors_scene, matches, 2 );    
    std::vector< cv::DMatch > good_matches;
    good_matches.reserve(matches.size());  

    for (size_t i = 0; i < matches.size(); ++i)
    { 
        if (matches[i].size() < 2)
            continue;

        const cv::DMatch &m1 = matches[i][0];
        const cv::DMatch &m2 = matches[i][1];

        if(m1.distance <= nndrRatio * m2.distance)        
            good_matches.push_back(m1);     
    }

    if( (good_matches.size() >=4))
    {
        //cout << "OBJECT FOUND!" << endl;
        std::vector< cv::Point2f > vobj;
        std::vector< cv::Point2f > vscene;

        for( unsigned int i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            vobj.push_back( keypointsO[ good_matches[i].queryIdx ].pt );
            vscene.push_back( keypointsS[ good_matches[i].trainIdx ].pt );
        }

        cv::Mat H = cv::findHomography( vobj, vscene, CV_RANSAC );

        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector< cv::Point2f > obj_corners(4);
        obj_corners[0] = cv::Point(0,0); obj_corners[1] = cv::Point( object.cols, 0 );
        obj_corners[2] = cv::Point( object.cols, object.rows ); obj_corners[3] = cv::Point( 0, object.rows );
        std::vector< cv::Point2f > scene_corners(4);

        cv::perspectiveTransform( obj_corners, scene_corners, H);

        //-- Draw lines between the corners (the mapped object in the scene - image_2 ) 
        cv::line( scene, scene_corners[0], scene_corners[1], CV_RGB(0, 0, 255), 2 ); //TOP line
        cv::line( scene, scene_corners[1], scene_corners[2], CV_RGB(0, 0, 255), 2 );
        cv::line( scene, scene_corners[2], scene_corners[3], CV_RGB(0, 0, 255), 2 );
        cv::line( scene, scene_corners[3], scene_corners[0], CV_RGB(0, 0, 255), 2 ); 

        int upperY = std::min( scene_corners[0].y, scene_corners[1].y );
        int leftX = std::min( scene_corners[0].x, scene_corners[3].x );
        int lowerY = std::max( scene_corners[2].y, scene_corners[3].y );
        int rightX = std::max( scene_corners[1].x, scene_corners[2].x );\
        
        foundRect = cv::Rect( cv::Point( leftX, upperY), cv::Point( rightX, lowerY ) );
    }
    
    return foundRect;
}

}
