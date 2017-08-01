ScaleTable = function(pixelOverSecodData, actualVelocityFireMH, tablevaluesMeters, NameColum){
scale = c()
for (i in pixelOverSecodData[[NameColum]]) {
  scale <- c(scale, (actualVelocityFireMH / (i*tablevaluesMeters)))
}
  return(cbind(pixelOverSecodData, scale))    
}
