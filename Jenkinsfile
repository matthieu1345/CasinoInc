pipeline {
    agent {
		label 'Unreal4212'
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
			discordSend description: "**Build:** ${env.BRANCH_NAME}\n**Status:** ${currentBuild.currentResult}\n\n**Changes:**\n\n${currentBuild.changeSets}", footer: '', image: '', link: env.BUILD_URL, result: currentBuild.currentResult, thumbnail: '', title: env.BRANCH_NAME, webhookURL: 'https://discordapp.com/api/webhooks/644642208624148480/YWBrqQ48ZAfsXvV6AuzJPFqwOQMJILKy1ihxzrl_XBbvzeyrj6LiTrTngDeV_mv_rx3K'
		}
	}
}