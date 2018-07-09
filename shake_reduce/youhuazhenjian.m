filename = 'cars2.avi';
vidObj = VideoReader(filename);
nFrames=vidObj.NumberOfFrames; 
obj_gray = VideoWriter('youhua.avi');
open(obj_gray);
if mod(nFrames,2)==0
        for i=1:2:nFrames
            frame = read(vidObj,i);
            frame1 = read(vidObj,i+1);
            gray_frame = (rgb2gray(frame)+rgb2gray(frame1))/2;
            %i_max=double(max(max(gray_frame)));  %获取亮度最大值
            %i_min=double(min(min(gray_frame)));  %获取亮度最小值
            %thresh=round(i_max-((i_max-i_min)/3));  %计算灰度图像转化成二值图像的门限thresh
            %gray_frame=(gray_frame>=thresh);  %I为二值图像
            %gray_frame=imadjust(gray_frame,[0.5,0.8],[0,0.2]);
            imshow(gray_frame);
            writeVideo(obj_gray, gray_frame);
            pause(0.01);  
        end
else
            for i=1:2:(nFrames-1)
            frame = read(vidObj,i);
            frame1 = read(vidObj,i+1);
            gray_frame = (rgb2gray(frame)+rgb2gray(frame1))/2;
            %i_max=double(max(max(gray_frame)));  %获取亮度最大值
            %i_min=double(min(min(gray_frame)));  %获取亮度最小值
            %thresh=round(i_max-((i_max-i_min)/3));  %计算灰度图像转化成二值图像的门限thresh
            %gray_frame=(gray_frame>=thresh);  %I为二值图像
            %gray_frame=imadjust(gray_frame,[0.5,0.6],[0,0.1]);
            imshow(gray_frame);
            writeVideo(obj_gray, gray_frame);
            pause(0.01);  
        end
end
close(obj_gray);
