filename = 'cars2.avi';
vidObj = VideoReader(filename);
nFrames=vidObj.NumberOfFrames; 
obj_gray = VideoWriter('dssqd.avi');
open(obj_gray);
if mod(nFrames,2)==0
        for i=1:2:nFrames-1
            frame = read(vidObj,i);
            %frame1 = read(vidObj,i+2);
            gray_frame = (rgb2gray(frame)+rgb2gray(frame1))/2;
            imshow(frame);
            writeVideo(obj_gray, gray_frame);
            pause(0.01);  
        end
else
        for i=1:2:nFrames
            frame = read(vidObj,i);
            %frame1 = read(vidObj,i+1);
            gray_frame = (rgb2gray(frame)+rgb2gray(frame1))/2;
            imshow(gray_frame);
            writeVideo(obj_gray, gray_frame);
            pause(0.01);  
        end
end
close(obj_gray);
