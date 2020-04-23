library('ggplot2')
library('reshape')
library('Cairo')
library("grid")

plottitle <- "Egeszet kiválaszt grafikon"
plotlabels <- c("1 szal", "2 szal", "4 szal", "8 szal")
xlabel <- "futtatás száma"
ylabel <- "Ido (s)"

data0 <- read.table('par1')
data1 <- read.table('par2')
data2 <- read.table('par4')
data3 <- read.table('par8')



d <- data.frame(c(1:length(data0$V1)),data0$V1, data1$V1, data2$V1, data3$V1)
colnames(d) <- c('ido', '1 szal', '2 szal', '4 szal', '8 szal')
d <- melt(d, id='ido', variable_name='series')


  
cairo_ps("test.eps", width=20, height=8)  
p<-ggplot(d, 
  aes_string(x=names(d)[1], y=names(d)[3], colour=names(d)[2]), 
  labeller=label_parsed) + 
  geom_point(size=4) + 
  geom_line(size=1.5) +
  labs(title=plottitle) + 
  xlab(xlabel) + 
  ylab(ylabel) + 
  scale_colour_manual(values=c("black", "blue", "red", "green", "purple"), name="", labels=plotlabels, guide=guide_legend(keyheight=unit(2, "line"), keywidth=unit(5, "line"))) +
  theme_gray(24) +
  theme(legend.position="bottom")+
  ylim(0.02, 0.04)+
  xlim(1, length(data0$V1))

print(p)
dev.off()
