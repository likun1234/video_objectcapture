filename = 'cars2.avi';
vidObj = VideoReader(filename);
nFrames=vidObj.NumberOfFrames; 
obj_gray = VideoWriter('zhenjianjunzhi.avi');
open(obj_gray);
        for i=1:nFrames-1
            frame = read(vidObj,i);
            frame1 = read(vidObj,i+1);
            gray_frame = (rgb2gray(frame)+rgb2gray(frame1))/2;
            %i_max=double(max(max(gray_frame)));  %获取亮度最大值
            %i_min=double(min(min(gray_frame)));  %获取亮度最小值

            %gray_frame=(gray_frame>=thresh);  %I为二值图像
            %gray_frame=imadjust(gray_frame,[0.5,0.8],[0,0.2]);
            %gray_frame=round(gray_frame/max(max(gray_frame))*120);
            imshow(gray_frame);
            writeVideo(obj_gray, gray_frame);

           %frame = (read(vidObj,i)+read(vidObj,i+1))/2;
     
            pause(0.01);  
        end
close(obj_gray);
