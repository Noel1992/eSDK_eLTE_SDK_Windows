
#ifndef DC_MEDIAMANAGER_H
#define DC_MEDIAMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include "sdk_export.h"

typedef enum {
    no_rotate =0,
    rotate_clockwise,
    rotate_flip,
    rotate_anticlockwise

}Rotate_Type_t;

typedef unsigned char uint8_t;

/**
\brief definition for a video YUV frame data
\note your application need destroy it after rendering the video frame
*/
typedef struct _YuvFrame{
        int w,h;            /** width and height */
        uint8_t *planes[4]; /** we usually use 3 planes, 4th is for compatibility */
        int strides[4];     /** with ffmpeg's swscale.h */
}YuvFrame;

/**
 \ brief Abstract interface for render control (can be like RTSP-style render control)
   or render control such as rotate, NOT USED ACTUALLY
*/
class RenderControl {
 public:
    virtual void rotate(Rotate_Type_t rotate) =0;
};

class VideoCapturer {
 public:

};

/**
\brief callback interface for VideoRenderer

*/
class VideoRenderer {
 public:

  /**
    \brief callback to set video size
  */
  virtual void SetSize(int width, int height, int reserved) = 0;

  /**
    \brief callback to set video resource ID
  */
  virtual void SetResourceID(MSISDN logical, MSISDN physical) = 0;

  /**
    \brief callback to set videochannel object for this Render
  */
  virtual void SetVideoChannel(void* vc) = 0;

  /**
    \brief callback API to render a new video frame
    \param frame: a YUV video frame
    \param physicalRid: resource ID of this frame
    \return: SDK NOT chech the return value
    \since: TTR2.0
  */
  virtual bool RenderYUVFrame(YuvFrame* frame, MSISDN physicalRid) = 0;

    /**
      \brief render control is NOT used, you can simplely return NULL
    */
    virtual RenderControl* getRenderControl() = 0;

    /**
      \brief get physical resource ID of this render
    */
    virtual MSISDN getPhysical()=0;
    /**
      \brief get physical resource ID of this render
      \note physical and logical resource ID is the same except the case where several video resources are rendered on the same window
    */
    virtual MSISDN getLogical()=0;

    /**
      \brief callback API to notify stop of rendering
      \note SDK will first call VideoRenderer::stopRendering() and next call MediaManager::del_renderer()
    */
    virtual void stopRendering()=0;

    /**
      \brief callback API to render a new video frame
      \param frame: a YUV video frame
      \param physicalRid: resource ID of this frame
      \param time_stat: video statistics
      \return: SDK NOT chech the return value
      \since: TTR3.0 SDK
      \note: the default implementation is invoking TTR2.0 RenderYUVFrame() to keep backward compatible
    */
    virtual bool RenderYUVFrame(YuvFrame* frame, MSISDN physicalRid, const void* time_stat) {return RenderYUVFrame(frame,physicalRid);}

    /**
      \brief callback API to notify video exceptions
      \param physicalRid: resource ID of this frame
      \return: SDK NOT chech the return value
      \since: TTR4.0
      \note: if your application doesn't care video exceptions, just keep the default implementation
    */
    virtual bool RenderYUVFrameException(MSISDN ) {return true;}

	//[TDTECH-jinglai.li-20151229]{
	/**
      \brief callback API to notify receive video rtp data
      \param physicalRid: resource ID of this frame
      \return: SDK NOT chech the return value
      \since: TTR4.0
      \note: 
    */

	virtual void notifyRecvFirstVideoData(MSISDN ) {return;}
	//[TDTECH-jinglai.li-20151229]}

	//[TDTECH-jinglai.li-20150314]{
	virtual void notifyRecordFileFull(const void* filepath ) {return;}
	//[TDTECH-jinglai.li-20150314]}
};

/**
 \brief Abstract interface for capture control (mainly PTZ)
 \note Not yet implemented
*/
class CaptureControl {
 public:

  virtual bool getPTZSupport() = 0;

  virtual bool SetResourceID(MSISDN rid) = 0;

  virtual bool doPTZcontrol(int cmdcode,int value) = 0;

};

/**
 \brief Abstract interface for construct video renderer and video capturer.
 \note most cases your application doesn't need render multi-resource, so you can simplely implement some API
       please see note for each API for more details
*/
class MediaManager {
 public:
    /**
      \brief callback API to query whether this video resource is to be rendered on the same window as other resources(Multi-Resource render)
      \note: you can simply return false if your application doesn't need handle Multi-Resource rendering
    */
    virtual bool isMultiResource(MSISDN resID) =0;
    /**
      \brief callback API to translate physical resource ID to its associated multi-resource ID
      \note: you can simply return the same physical ID if your application doesn't need handle Multi-Resource rendering
    */
    virtual int getLogicalResid(MSISDN physicalResid) =0;
    /**
      \brief return render object for a resource
      \note your application need maintain the association between resource ID and render object
    */
    virtual VideoRenderer* getRender(MSISDN resID) =0;
    /**
      \brief return render object for a multi_resource, only applicable to multi-resource rendering
    */
    virtual VideoRenderer* getRender4MultiResource(MSISDN resID) =0;

    /**
      \brief callback API to create a render object for a multi-resource, only used in video circulate
      \param resID: ID of the video resource
      \note: only applicable for multiple video resources to be rendered in one window, such as video circulating
    */
    virtual VideoRenderer* new_renderer4MultiResource(MSISDN resID) =0;

    /**
      \brief callback API to create a render object for a regular video resource
      \param resID: ID of the video resource
      \note: at the begining of a video session, SDK will call this API to notify the start of a video stream
    */
    virtual VideoRenderer* new_renderer( MSISDN resID) = 0;

    /**
      \brief callback API to destruct one VideoRenderer object for a video resource.
      \param resID: ID of the video resource
      \note: at the end of a video session, SDK will call this API to notify the end of a video stream
             3rd party application shall associate a resource ID with a renderobject, and delete the renderobject in this callback API
    */
    virtual void del_renderer( MSISDN resID) =0;

};

#endif // DC_MEDIAMANAGER_H
