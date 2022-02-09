/*
g++ -I/usr/include/libxml2 test_xml.c -lxml2
*/
#include <stdio.h>
#include <string.h>
// 
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

/*
<?xml version="1.0"?>
<story>
  <storyinfo>
    <author>John Fleck</author>
    <datewritten>June 2, 2002</datewritten>
    <keyword>example keyword</keyword>
  </storyinfo>
  <body>
    <headline>This is the headline</headline>
    <para>This is the body text.</para>
  </body>
</story>
*/

void
parseStory (xmlDocPtr doc, xmlNodePtr cur) 
{
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keyword"))) 
	    {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("keyword: %s\n", key);
		    xmlFree(key);
 	    }
	    cur = cur->next;
	}
    return;
}

static void
parseDoc(char* szBuffer) 
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(szBuffer);
	/*
	doc = xmlParseMemory(szBuffer, strlen(szBuffer));
	*/
	
	if (doc == NULL ) 
	{
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) 
	{
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "story")) 
	{
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
    {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo")))
		{
			parseStory (doc, cur);
		}
		 
	    cur = cur->next;
	}
	
	xmlFreeDoc(doc);
	return;
}

int main(int argc, char **argv)
{
    // 
    parseDoc("test_xml.xml");
    exit(0);
    /*
    // 
    xmlDocPtr doc;
    xmlNodePtr cur;
    // 
    doc = xmlParseMemory(szBuffer, strlen(szBuffer));
    if(NULL == doc) 
    {
        printf("Document not parsed successfully. \n");
        exit(0);
    }
    
    // 
    cur = xmlDocGetRootElement(doc);
    if(NULL == cur) 
    {
        printf("Document is empty. \n");
        xmlFreeDoc(doc);
        exit(0);
    }
    
    // 
    printf("root:[%s].\n", cur->name);
    // 
    cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	{
	    printf("node:[%s].\n", cur->name);
	    cur = cur->next;
	}    
    
    // 
    xmlFreeDoc(doc);
    
    //printf("%s\n", szBuffer);
    exit(0);
    */
}
