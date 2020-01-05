String GetStatusPic() {
  String imgString = F("<svg height='64' width='64'>");
  imgString += F("  <circle cx='32' cy='32' r='30' stroke='black' stroke-width='3' style='fill:");

  if (realWhite > 0) {
    imgString += F("white");
  } else {
    imgString += F("rgb(");
    imgString += realRed;
    imgString += F(",");
    imgString += realGreen;
    imgString += F(",");
    imgString += realBlue;
    imgString += F(")");
  }
  
  imgString += F("' />");
  imgString += F("  Sorry, your browser does not support inline SVG.");
  imgString += F("</svg>");
  return imgString;
}

void ServeWebClients() 
{
  String inString = F("<head><title>");
  inString += deviceName;
  inString += F("</title>");
  inString += F("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  inString += F("<link rel='stylesheet' href='http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.css' type='text/css'>");
  inString += F("<script src='http://code.jquery.com/jquery-1.9.1.min.js' type='text/javascript'></script>");
  inString += F("<script src='http://code.jquery.com/mobile/1.3.1/jquery.mobile-1.3.1.min.js' type='text/javascript'></script>");
  inString += F("</head>");
  inString += F("<body>");

  inString += F("<div data-role='page' id='page_opts'>");
  inString += F("<div data-role='header'><h3>");
  inString += deviceName;
  inString += F("</h3></div>");
  inString += F("<div data-role='content'>");

  inString += F("<fieldset data-role='controlgroup' data-type='horizontal'>");
  inString += F("<input type='radio' name='opt_group' id='basic' onclick='toggle_opt()' checked><label for='basic'>Info</label>");
  inString += F("<input type='radio' name='opt_group' id='other' onclick='toggle_opt()'><label for='other'>Settings</label>");
  inString += F("</fieldset>");
  
  
  inString += F("<div id='div_basic'>");
  
  inString += F("<h3 class='ui-bar ui-bar-a ui-corner-all'>LED Status</h3>");
  inString += F("<table><tr><td><b>State:<br></td><td><label id='lbl_status' ");
  if (stateOn) {
    inString += F("style='color:green;font-size:larger;font-weight:600;'>ON");
  } else {
    inString += F("style='color:red;font-size:larger;font-weight:600;'>OFF");
  }
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td><b>Red:</b></td><td><label id='lbl_red'>");
  inString += realRed;
  inString += F("</label></td><td rowspan='4'>");
  inString += GetStatusPic();
  inString += F("</td></tr>");     
  inString += F("<tr><td><b>Green:</b></td><td><label id='lbl_green'>");
  inString += realGreen;
  inString += F("</label></td></tr>");
  inString += F("<tr><td><b>Blue:</b></td><td><label id='lbl_blue'>");
  inString += realBlue;
  inString += F("</label></td></tr>");
  inString += F("<tr><td><b>White:</b></td><td><label id='lbl_white'>");
  inString += realWhite;
  inString += F("</label></td></tr>");

  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  inString += F("<tr><td><b>Brightness:</b></td><td><label id='lbl_britness'>");
  inString += brightness;
  inString += F("</label></td></tr>");  
  inString += F("<tr><td><b>TransitionTime:</b></td><td><label id='lbl_tt'>");
  inString += transitionTime;
  inString += F("</label></td></tr>");
  inString += F("<tr><td><b>Effect:</b></td><td><label id='lbl_effect'>");
  inString += effect.c_str();
  inString += F("</label></td></tr>");
  inString += F("<tr><td><b>Transition:</b></td><td><label id='lbl_effect'>");
  if (transitionDone) {
    inString += F("Done");
  } else {
    inString += F("Running");
  }  
  inString += F("</label></td></tr>");
      
  inString += F("</table><br />");
  inString += F("</div>");

  
  inString += F("<div id='div_other' style='display:none;'>");
  inString += F("<table cellpadding='2'>");
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");

  inString += F("<tr><td><b>Device Name:</b></td><td><label id='lbl_deviceName'>");
  inString += deviceName;
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td><b>LED Count:</b></td><td><label id='lbl_ledCount'>");
  inString += ledCount;
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td><b>Max Brightness:</b></td><td><label id='lbl_maxBrightness'>");
  inString += maxBrightness;
  inString += F(" (255 Max)</label></td></tr>");

  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");
  
  inString += F("<tr><td><b>WIFI SSID:</b></td><td><label id='lbl_rssi'>");
  inString += WIFI_SSID;
  inString += F("</label></td></tr>");

  inString += F("<tr><td><b>WiFi&nbsp;Signal:</b></td><td><label id='lbl_rssi'>");
  long rssi = WiFi.RSSI();
  if (rssi > -71) {
    inString += F("Good");
  } else if (rssi > -81) {
    inString += F("Weak");    
  } else {
    inString += F("Poor");    
  }
  inString += F(" (");
  inString += rssi;
  inString += F(" dbBm)");
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");

  inString += F("<tr><td><b>MQTT Server:</b></td><td><label id='lbl_rssi'>");
  inString += MQTT_SERVER;
  inString += F("</label></td></tr>");
  
  inString += F("<tr><td><b>Topic:</b></td><td><label id='lbl_rssi'>");
  inString += MQTT_STATE_TOPIC_PREFIX;
  inString += deviceName;
  inString += F("</label></td></tr>");
  inString += F("<tr><td colspan='2'>&nbsp;</td></tr>");

  inString += F("</table>");
  inString += F("</div>");

  
  inString += F("<div data-role='footer' data-theme='c'><h5>");
  inString += VERSION;
  inString += F("</h5></div>");
  inString += F("</div>");

  inString += F("<script>");
  inString += F("function eval_cb(n)  {return $(n).is(':checked')?1:0;}");
  inString += F("function toggle_opt() {");
  inString += F("$('#div_basic').hide();");
  inString += F("$('#div_other').hide();");
  inString += F("if(eval_cb('#basic')) $('#div_basic').show();");
  inString += F("if(eval_cb('#other')) $('#div_other').show();");
  inString += F("};");
  inString += F("</script>");
  inString += F("</body>");

  server.send(200, "text/html", inString);
}
