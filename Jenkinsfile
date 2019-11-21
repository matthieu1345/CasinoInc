pipeline {
    agent {
		label 'Unreal4212'
	}
	options {
		buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
	}
	
    stages{
        stage('scm'){
			steps{
				checkout scm
			}
		}
		stage('build'){
			stages{
				stage('BuildTool-Prep'){
					steps{
						bat label: 'Prepare', script: '"%Unreal 4.21.2_HOME%\\%Unreal_BuildTool%" -projectfiles -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -game -rocket -progress'
					}
				}
				stage('Build'){
					steps{
						echo 'hi'
						//bat label: 'Build', script: '"%Unreal 4.21.2_HOME%\\%Unreal_RunUAT%" BuildCookRun -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory="%WORKSPACE%\\Builds"'
					}
				}
				stage('Cook'){
					steps{
						echo 'hi'
						//bat label: 'cook', script: '"%Unreal 4.21.2_HOME%\\%Unreal_RunUAT%" BuildCookRun -project="%WORKSPACE%\\Casino_Inc\\Casino_Inc.uproject" -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -NoCompile -stage -pak -archive -archivedirectory="%WORKSPACE%\\Builds"'
					}
				}
			}
		}
    }
	post {
		always {
			//archiveArtifacts 'Builds/**/*.*'
			
			sendDiscord()
		}
	}
}

def sendDiscord(){
	def changeString = ""
	
	def changeSets = currentBuild.changeSets
	for (int i = 0; i < changeSets.size(); i++){
		def entries = changeSets[i].items
		for (int j = 0; j < entries.length; j++){
			def entry = entries[j]
			truncated_ID = entry.commitId.take(7)
			truncated_msg = entry.msg.take(100)
			changeString += "\n- `${truncated_ID}` *${truncated_msg} -  ${entry.author}*"
		}
	}
	
	if (!changeString) {
        changeString = "\n\n - No new changes"
    }
    
	discordSend description: "**Build:** ${env.BRANCH_NAME}\n**Status:** ${currentBuild.currentResult}\n\n**Changes:**${changeString}\n\n**Artifacts:**\n- ${env.BUILD_URL}/artifact/",
	footer: '',
	image: '',
	link: env.BUILD_URL,
	result: currentBuild.currentResult,
	thumbnail: '',
	title: env.BRANCH_NAME,
	webhookURL: 'https://discordapp.com/api/webhooks/644642208624148480/YWBrqQ48ZAfsXvV6AuzJPFqwOQMJILKy1ihxzrl_XBbvzeyrj6LiTrTngDeV_mv_rx3K'
}