clear

fs=24e3;

allout=[];
f = figure;
f.Position = [100 100 540 1000];

mout=dlmread('mout.txt');
mout=bandpass(mout,[220,230],fs);

pout=dlmread('pout.txt');
ref=pout(1);
pout=pout(2:end);
pout=pout-ref;

fname_time=1614887141910;

fid=fopen('sout.txt');
tline = fgetl(fid);
while ischar(tline)
   if contains(tline,'maxtime')
       maxtime=str2num(string(extractBetween(tline,9,9+12)));
       maxtime=maxtime-fname_time;
       maxtime=(maxtime/1000)*fs;
   elseif contains(tline,'mintime')
       mintime=str2num(string(extractBetween(tline,9,9+12)));
       mintime=mintime-fname_time;
       mintime=(mintime/1000)*fs;
   elseif contains(tline,'sealtime')
       sealtime=str2num(string(extractBetween(tline,10,10+12)));
       sealtime=sealtime-fname_time;
       sealtime=(sealtime/1000)*fs;
   end
   tline = fgetl(fid);
end
fclose(fid);

[yup,ydown] = envelope(mout);

subplot(211)
hold on
tt=linspace(0,length(mout)/fs,length(mout));
plot(tt,mout);
plot(tt,yup);
xlabel('Time (s)')
ylabel('Amplitude')

% find start point
[~,min_idx]=min(yup(maxtime-fs/2:maxtime));
min_idx=min_idx+maxtime-fs/2;
[~,max_idx]=max(yup(min_idx:maxtime));
start_point=max_idx+min_idx;

%find end point
[~,min_idx]=min(yup(mintime-fs/2:mintime));
min_idx=min_idx+mintime-fs/2;
[~,max_idx]=max(yup(mintime-fs/2:min_idx));
end_point=max_idx+mintime-fs/2;

xline(tt(start_point),'LineWidth',2);
xline(tt(end_point),'LineWidth',2);
xlim([0,tt(end)])

[~,max_idx]=max(pout);
[~,min_idx]=min(pout);
pout=pout(max_idx:min_idx);

mout=mout(start_point:end_point);

micValsPerBin=int32(floor(length(mout)/length(pout)));
cc=1;
mags=[];
for j=1:length(pout)
  seg=mout(cc:cc+micValsPerBin); 
  cc=cc+micValsPerBin;
  spec=abs(fft(seg,24000));
  mags=[mags 1/spec(227)];
end
[~,max_pidx]=max(mags);

%% interpolate
mags2=[];
pout2=[];
for k=1:length(pout)-1
   mags2=[mags2 mags(k) (mags(k)+mags(k+1))/2];
   pout2=[pout2 pout(k) (pout(k)+pout(k+1))/2];
end
mags2=[mags2 mags(end)];
pout2=[pout2 pout(end)];

%% discretize
mykeys=-450:5:200;
pout2=round(pout2/5)*5;

edges = containers.Map('KeyType','double', 'ValueType','double');
for k=1:length(mykeys)
  edges(mykeys(k))=0; 
end

for k=1:length(pout)
  edges(pout(k)) = mags(k);
end

for k=2:length(mykeys)-1
   if edges(mykeys(k)) == 0
       if edges(mykeys(k+1)) ~= 0
          edges(mykeys(k)) = edges(mykeys(k+1)); 
       else
          edges(mykeys(k)) = edges(mykeys(k-1));
       end
   end
end

bound1 = 11;
bound2 = length(mykeys);

nkeys=zeros(bound2-bound1,1);
counter=1;
for k=bound1:bound2
   nkeys(counter)=mykeys(k);
   counter=counter+1;
end

nvalues=zeros(length(nkeys),1);
for k=1:length(nkeys)
   nvalues(k)=edges(nkeys(k)); 
end

mags2=movmean(mags2,5);

%% scale
coeffs=[7.1522,-12.2250,8.7021,0];
init=1.4354e-07;
mmax=1.0434e-06;
normed = (mags2-init)/mmax;
outs = [];

for val=normed
   out=0;
   for j=1:length(coeffs)
      out=out+coeffs(j)*(val.^(length(coeffs)-j));
   end
   outs=[outs out];
end

vea=outs(end);
outs=outs-vea;

ml=max(outs);
[~,dapa]=max(outs);
dapa=pout2(dapa);
allout=[allout;vea ml dapa];

subplot(212)
plot(pout2,outs)
xlim([-400,200])
ylim([-.5,2])
text(-390,1.7,sprintf("Ear canal volume: %.2f mL\nAdmittance: %.2f mL\nPeak pressure: %d daPa",vea,ml,int32(dapa)))
xlabel('Pressure (daPa)')
ylabel('Admittance (mL)')






























