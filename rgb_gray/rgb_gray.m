clear
clc
file_name = '2cars.avi';        
obj = VideoReader(file_name);     
obj_gray = VideoWriter('car2.avi');
open(obj_gray);
numFrames = obj.NumberOfFrames; 
for k = 1: numFrames
frame = read(obj,k);
subplot(1,2,1);
imshow(frame);                
gray_frame = rgb2gray(frame); 
subplot(1,2,2);
imshow(gray_frame);
writeVideo(obj_gray, gray_frame);
end
close(obj_gray);

