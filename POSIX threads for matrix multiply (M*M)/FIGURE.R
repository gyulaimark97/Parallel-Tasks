library('ggplot2')
library('reshape')
library('Cairo')
library("grid")

plottitle <- "Egeszet kiválaszt grafikon"
plotlabels <- c("futas")
xlabel <- "futtatás száma"
ylabel <- "Ido (s)"

data0 <- read.table('par')

d <- data.frame(c(1:length(data0$V1)),data0$V1)
colnames(d) <- c('ido', 'futas')
d <- melt(d, id='ido', variable_name='series')


  
cairo_ps("kep.eps", width=20, height=8)  
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
  xlim(1, length(data0$V1))

print(p)
dev.off()
