
loop
  while (buffers are not empty)
    
    if (WiFi is connected)
      Send data to the server
      
      if (sending is successful)
        remove data from buffer
      else
        display "Sending data to server failed"
        break
    else
      break
    
  while (read_count < rounds)
    read sensor values

  calculate mean and standard deviation of each sensor types

  if (WiFi is connected)
    Send data to the server
    
    if (sending is not successful)
      push sensor values to the buffer
  
  else
    reconnect to WiFi
    
    if (WiFi is connected)
      Send data to the server
      
      if (sending is not successful)
        push sensor values to the buffer

    else
      push sensor values to the buffer


