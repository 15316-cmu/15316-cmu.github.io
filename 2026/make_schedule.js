
for(var i = 0; i < schedule.length; i++) {
  ent = schedule[i];
  
  ent_container = jQuery('<tr/>')
  if('color' in ent)
    $(ent_container).addClass('table-' + ent['color'])

  date = jQuery('<td>' + ent['date'] + '</td>')
  topic = jQuery('<td>' + ent['topic'] + '</td>')
  
  links = jQuery('<td/>')
  if(ent['pdf'].length > 0) {
    pdf = jQuery('<a href=\"lectures/' + ent['pdf'] + '\">[pdf]</a>')
    links.append(pdf)
  }
  if(ent['tex'].length > 0) {
    if(ent['pdf'].length > 0) {
      links.append(' ')
    }
    tex = jQuery('<a href=\"lectures/' + ent['tex'] + '\">[tex]</a>')
    links.append(tex)
  }

  comments = jQuery('<td>' + ent['comments'] + '</td>')

  ent_container.append(date)
  ent_container.append(topic)
  ent_container.append(links)
    ent_container.append(comments)

    $("#schedule").append(ent_container)

/*
    if (i % 2 == 1) {
        blank = jQuery('<tr> <td colspan=4 style="height: 0px;"></td> </tr>')
        $("#schedule").append(blank)
    }
*/

}
